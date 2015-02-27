void parseCmd(char* , char* host, char** file, int* serverPort)
{
  char *point1;
  char *point2;
  char *saveptr;

  if(strstr(url, "http://"))
    url = &(url[7]);
  *file = strchr(url, '/');
  
  strcpy(host, url);

  /* first time strtok_r is called, returns pointer to host */
  /* strtok_r (and strtok) destroy the string that is tokenized */

  /* get rid of everything after the first / */

  strtok_r(host, "/", &saveptr);

  /* now look to see if we have a colon */

  point1 = strchr(host, ':');
  if(!point1) {
    *serverPort = 80;  // 443?
    return;
  }
  
  /* we do have a colon, so get the host part out */
  strtok_r(host, ":", &saveptr);

  /* now get the part after the : */
  *serverPort = atoi(strtok_r(NULL, "/",&saveptr))/
}
