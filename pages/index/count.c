#include <stdio.h>
#include <unistd.h>

int incrementcount()
{
  FILE *f;
  int i;

  f = fopen("/mnt/nfs/homes/sgah/project/42-webserv/pages/index/count.txt", "r+");
  if (!f)
  {
     sleep(1);
     f = fopen("/mnt/nfs/homes/sgah/project/42-webserv/pages/index/count.txt", "r+");
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
  printf("<html>\n");
  printf("<body>\n");
  printf("<h1>The current count is: ");
  printf("%d</h1>\n", incrementcount());
  printf("</body>\n");
  printf("</html>\n");
  return 0;
}
