//
//  main.c
//  MqttClient
//
//  Created by Manuel Schreiner on 16.04.20.
//  Copyright © 2020 io-expert.com. All rights reserved.
//

#include <stdio.h>
/*
  needs libmosquitto-dev
  $ gcc -o libmosquittoHandle libmosq.c -lmosquitto
*/
#include <stdio.h>
#include <mosquitto.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* strTopic = "test/topic";
char* strHostname = "localhost";
char* strMessage = NULL;
int   intPort = 1883;

struct mosquitto *mosquittoHandle = NULL;


void mosq_log_callback(struct mosquitto *mosquittoHandle, void *userdata, int level, const char *str)
{
  switch(level){
    case MOSQ_LOG_DEBUG:
          break;
    case MOSQ_LOG_INFO:
          break;
    case MOSQ_LOG_NOTICE:
          break;
    case MOSQ_LOG_WARNING:
          break;
    case MOSQ_LOG_ERR:
          printf("%i:%s\n", level, str);
          break;
    default:
          printf("UNKNOWN: %i:%s\n", level, str);
          break;
    }
}


void mqtt_setup(){
  int keepalive = 60;
  bool clean_session = true;

  mosquitto_lib_init();
  mosquittoHandle = mosquitto_new(NULL, clean_session, NULL);
  if(!mosquittoHandle){
        fprintf(stderr, "Error: Out of memory.\n");
        exit(1);
    }
  
  mosquitto_log_callback_set(mosquittoHandle, mosq_log_callback);
  
  if(mosquitto_connect(mosquittoHandle, strHostname, intPort, keepalive)){
        fprintf(stderr, "Unable to connect.\n");
        exit(1);
    }
  int loop = mosquitto_loop_start(mosquittoHandle);
  if(loop != MOSQ_ERR_SUCCESS){
    fprintf(stderr, "Unable to start loop: %i\n", loop);
    exit(1);
  }
}

int mqtt_send(char *msg){
  return (int)mosquitto_publish(mosquittoHandle, NULL, strTopic, (uint32_t)strlen(msg), msg, 0, 0);
}

int main(int argc, char *argv[])
{
  for(int i=1;i < argc;i++)
  {
      if ((i+1) < argc)
      {
          printf("Using ");
          if (strcmp(argv[i],"-p") == 0)
          {
              intPort = atoi(argv[i+1]);
              printf("with port %d ",intPort);
          } else if (strcmp(argv[i],"-h") == 0)
          {
              strHostname = argv[i+1];
              printf("with host %s ",strHostname);
          } else if (strcmp(argv[i],"-t") == 0)
          {
              strTopic = argv[i+1];
              printf("with topic %s ",strTopic);
          } else if (strcmp(argv[i],"-m") == 0)
          {
              strMessage = argv[i+1];
              printf("with message %s",strMessage);
          }
          printf("\n");
          i++;
      }
  }
  mqtt_setup();
  if (strMessage != NULL)
  {
      mqtt_send(strMessage);
      exit(0);
  } else
  {
      int i = 0;
      char *buf = malloc(64);
      while(1){
        sprintf(buf,"i=%i",i++);
        int snd = mqtt_send(buf);
        if(snd != 0) printf("mqtt_send error=%i\n", snd);
        usleep(100000);
      }
  }
}
