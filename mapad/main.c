 #include "mapad.h"
struct mapad_conf conf;

int main(int argc, char **argv)
{
	int ret ;
	pthread_t chart;

	pthread_create(&chart, NULL, input_thread, (void *)NULL);

	if(argc != 3)
	{
		printf(USAGE_STRING) ;
		exit(0);
	}
	if(strcmp(argv[1], "-C") == 0)
	{
		conf.conf_file = argv[2];
		reload_config();
	}
	if(conf.mode == MAPD_MODE_CLIENT)
	{
		printf("Starting MAPAD in client mode\n") ;
		ret = start_client();
	}
	else if(conf.mode == MAPD_MODE_SERVER)
	{
		printf("Starting MAPAD in Server Mode\n") ;
		start_server();
	}
	while(1)
	{
		sleep(500000);
	}
	exit(1);
}
