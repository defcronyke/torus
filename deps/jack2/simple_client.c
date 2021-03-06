/*  Copyright (c) 2021 Jeremy Carter <jeremy@jeremycarter.ca>,
    and possibly anyone listed in the file:
    deps/jack2/AUTHORS.rst

    Borrowed and then modified from:
    https://github.com/jackaudio/jack2/blob/1ab3445e65ec2bfc1ba8562984822616e03fb9fe/example-clients/simple_client.c

    All uses of this project in part or in whole are governed
    by the terms of the license contained in the file titled
    "LICENSE" that's distributed along with the project, which
    can be found in the top-level directory of this project.

    If you don't agree to follow those terms or you won't
    follow them, you are not allowed to use this project or
    anything that's made with parts of it at all. The project
    is also	depending on some third-party technologies, and
    some of those are governed by their own separate licenses,
    so furthermore, whenever legally possible, all license
    terms from all of the different technologies apply, with
    this project's license terms taking first priority.
*/

/** @file simple_client.c
 *
 * @brief This simple client demonstrates the basic features of JACK
 * as they would be used by many applications.
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>
#include <stdatomic.h>
#ifndef WIN32
#include <unistd.h>
#include <ncurses.h>
#endif
#include <jack/jack.h>

atomic_int jack2_client_running = 1;

jack_port_t *input_port1, *input_port2;
jack_port_t *output_port1, *output_port2;
jack_client_t *client;

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

#define TABLE_SIZE   (200)
typedef struct
{
	float sine[TABLE_SIZE];
	int left_phase;
	int right_phase;
}
paTestData;

static void signal_handler(int sig)
{
	jack_client_close(client);
	fprintf(stderr, "signal received, exiting ...\n");
  jack2_client_running = 0;
	// exit(0);
}

/**
 * The process callback for this JACK application is called in a
 * special realtime thread once for each audio cycle.
 *
 * This client follows a simple rule: when the JACK transport is
 * running, copy the input port to the output.  When it stops, exit.
 */

int process (jack_nframes_t nframes, void *arg) {
  jack_default_audio_sample_t *in1, *in2;
	jack_default_audio_sample_t *out1, *out2;
	
  paTestData *data = (paTestData*)arg;
	
  int i;

  in1 = (jack_default_audio_sample_t*)jack_port_get_buffer (input_port1, nframes);
	in2 = (jack_default_audio_sample_t*)jack_port_get_buffer (input_port2, nframes);


#ifndef _WIN32
  refresh();
  mvprintw(7, 0, "in1[nframes/2] = %.15f              ", in1[nframes/2]);
  mvprintw(8, 0, "in2[nframes/2] = %.15f              ", in2[nframes/2]);
  mvprintw(9, 0, "");
  mvprintw(10, 0, "");
  refresh();
#else
  printf("in1[nframes/2] = %.15f\n", in1[nframes/2]);
  printf("in2[nframes/2] = %.15f\n\n", in2[nframes/2]);
#endif


	out1 = (jack_default_audio_sample_t*)jack_port_get_buffer (output_port1, nframes);
	out2 = (jack_default_audio_sample_t*)jack_port_get_buffer (output_port2, nframes);

	for( i=0; i<nframes; i++ )
	{
		out1[i] = data->sine[data->left_phase];  /* left */
		out2[i] = data->sine[data->right_phase];  /* right */
		data->left_phase += 1;
		if( data->left_phase >= TABLE_SIZE ) data->left_phase -= TABLE_SIZE;
		data->right_phase += 3; /* higher pitch so we can distinguish left and right. */
		if( data->right_phase >= TABLE_SIZE ) data->right_phase -= TABLE_SIZE;
	}
    
	return 0;      
}

/**
 * JACK calls this shutdown_callback if the server ever shuts down or
 * decides to disconnect the client.
 */
void jack_shutdown (void *arg)
{
  printf("jack2 client shutting down...");


  printf("jack2 client stopped");
	// exit (1);

  jack2_client_running = 0;
}

int jack2_simple_client_main(int argc, char *argv[]) {
  double volume = 0.01;

	const char **ports;
	const char *client_name;
	const char *server_name = NULL;
	jack_options_t options = JackNullOption;
	jack_status_t status;
	paTestData data;
	int i;

	if (argc >= 2) {		/* client name specified? */
		client_name = argv[1];
		if (argc >= 3) {	/* server name specified? */
			server_name = argv[2];
			int my_option = JackNullOption | JackServerName;
			options = (jack_options_t)my_option;
		}
	} else {			/* use basename of argv[0] */
		client_name = strrchr(argv[0], '/');
		if (client_name == 0) {
			client_name = argv[0];
		} else {
			client_name++;
		}
	}

	for( i=0; i<TABLE_SIZE; i++ )
	{
		data.sine[i] = volume * (0.2 * (float) sin( ((double)i/(double)TABLE_SIZE) * M_PI * 2. ));
	}
	data.left_phase = data.right_phase = 0;
  

	/* open a client connection to the JACK server */

	client = jack_client_open (client_name, options, &status, server_name);
	if (client == NULL) {
		fprintf (stderr, "jack_client_open() failed, "
			 "status = 0x%2.0x\n", status);
		if (status & JackServerFailed) {
			fprintf (stderr, "Unable to connect to JACK server\n");
		}

    return 1;
		// exit (1);
	}
	if (status & JackServerStarted) {
		fprintf (stderr, "JACK server started\n");
	}
	if (status & JackNameNotUnique) {
		client_name = jack_get_client_name(client);
		fprintf (stderr, "unique name `%s' assigned\n", client_name);
	}

	/* tell the JACK server to call `process()' whenever
	   there is work to be done.
	*/

	jack_set_process_callback (client, process, &data);

	/* tell the JACK server to call `jack_shutdown()' if
	   it ever shuts down, either entirely, or if it
	   just decides to stop calling us.
	*/

	jack_on_shutdown (client, jack_shutdown, 0);

  /* create two input ports */

	input_port1 = jack_port_register (client, "input1",
					  JACK_DEFAULT_AUDIO_TYPE,
					  JackPortIsInput, 0);

	input_port2 = jack_port_register (client, "input2",
					  JACK_DEFAULT_AUDIO_TYPE,
					  JackPortIsInput, 0);

	if ((input_port1 == NULL) || (input_port2 == NULL)) {
		fprintf(stderr, "no more JACK input ports available\n");
    return 1;
		// exit (1);
	}

	/* create two output ports */

	output_port1 = jack_port_register (client, "output1",
					  JACK_DEFAULT_AUDIO_TYPE,
					  JackPortIsOutput, 0);

	output_port2 = jack_port_register (client, "output2",
					  JACK_DEFAULT_AUDIO_TYPE,
					  JackPortIsOutput, 0);

	if ((output_port1 == NULL) || (output_port2 == NULL)) {
		fprintf(stderr, "no more JACK output ports available\n");
    return 1;
		// exit (1);
	}

	/* Tell the JACK server that we are ready to roll.  Our
	 * process() callback will start running now. */

	if (jack_activate (client)) {
		fprintf (stderr, "cannot activate client");
    return 1;
		// exit (1);
	}

	/* Connect the output ports.  You can't do this before the client is
	 * activated, because we can't make connections to clients
	 * that aren't running.  Note the confusing (but necessary)
	 * orientation of the driver backend ports: playback ports are
	 * "input" to the backend, and capture ports are "output" from
	 * it.
	 */
 	
	// ports = jack_get_ports (client, NULL, NULL,
	// 			JackPortIsPhysical|JackPortIsInput);
	// if (ports == NULL) {
	// 	fprintf(stderr, "no physical playback ports\n");
  //   return 1;
	// 	// exit (1);
	// }

	// if (jack_connect (client, jack_port_name (output_port1), ports[0])) {
	// 	fprintf (stderr, "cannot connect output ports\n");
	// }

	// if (jack_connect (client, jack_port_name (output_port2), ports[1])) {
	// 	fprintf (stderr, "cannot connect output ports\n");
	// }

	// jack_free (ports);


  /* Connect the input ports.  You can't do this before the client is
	 * activated, because we can't make connections to clients
	 * that aren't running.  Note the confusing (but necessary)
	 * orientation of the driver backend ports: capture ports are
	 * "output" to the backend, and playback ports are "input" from
	 * it.
	 */

  // ports = jack_get_ports (client, NULL, NULL,
	// 			JackPortIsPhysical|JackPortIsOutput);
	// if (ports == NULL) {
	// 	fprintf(stderr, "no physical capture ports\n");
  //   return 1;
	// 	// exit (1);
	// }

	// if (jack_connect (client, jack_port_name (input_port1), ports[0])) {
	// 	fprintf (stderr, "cannot connect input ports\n");
	// }

	// if (jack_connect (client, jack_port_name (input_port2), ports[1])) {
	// 	fprintf (stderr, "cannot connect input ports\n");
	// }

	// jack_free (ports);
    
    /* install a signal handler to properly quits jack client */
#ifdef WIN32
	signal(SIGINT, signal_handler);
	signal(SIGABRT, signal_handler);
	signal(SIGTERM, signal_handler);
#else
	signal(SIGQUIT, signal_handler);
	signal(SIGTERM, signal_handler);
	signal(SIGHUP, signal_handler);
	signal(SIGINT, signal_handler);
#endif

	/* keep running until the Ctrl+C */

	while (jack2_client_running == 1) {
	#ifdef WIN32 
		Sleep(1000);
	#else
		sleep (1);
	#endif
	}

	jack_client_close (client);

  return 0;
	// exit (0);
}
