#include <stdio.h>
#include <unistd.h>

int incrementcount()
{
  FILE *f;
  int i;

  f = fopen("/mnt/nfs/homes/vdescham/Documents/42-webserv/DOC/POC/vdescham/CGI/CGI_test/count.txt", "r+");
  if (!f)
  {
     sleep(1);
     f = fopen("/mnt/nfs/homes/vdescham/Documents/42-webserv/DOC/POC/vdescham/CGI/CGI_test/count.txt", "r+");
     if (!f)
       return -1;
  }

  fscanf(f, "%d", &i);
  i++;
  fseek(f,0,SEEK_SET);
  fprintf(f, "%d", i);
  fclose(f);
  return i;
}

int main()
{
  printf("Content-type: text/html\n\n");
  printf("<html>\n");
  printf("<body>\n");
  printf("<h1>The current count is: ");
  printf("%d</h1>\n", incrementcount());
  printf("</body>\n");
  printf("</html>\n");
  return 0;
}
