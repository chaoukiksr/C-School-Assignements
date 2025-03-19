#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

// Modified to handle permissions and file type
void print_file_attributes(struct stat *file_status, char *file_name)
{
   char perm[] = "----------"; // 10 chars + null terminator

   // Determine file type
   if (S_ISDIR(file_status->st_mode))
      perm[0] = 'd';
   else if (S_ISLNK(file_status->st_mode))
      perm[0] = 'l';
   else if (S_ISBLK(file_status->st_mode))
      perm[0] = 'b';
   else if (S_ISCHR(file_status->st_mode))
      perm[0] = 'c';
   else if (S_ISFIFO(file_status->st_mode))
      perm[0] = 'p';
   else if (S_ISSOCK(file_status->st_mode))
      perm[0] = 's';

   // Set permissions
   perm[1] = (file_status->st_mode & S_IRUSR) ? 'r' : '-';
   perm[2] = (file_status->st_mode & S_IWUSR) ? 'w' : '-';
   perm[3] = (file_status->st_mode & S_IXUSR) ? ((file_status->st_mode & S_ISUID) ? 's' : 'x') : ((file_status->st_mode & S_ISUID) ? 'S' : '-');

   perm[4] = (file_status->st_mode & S_IRGRP) ? 'r' : '-';
   perm[5] = (file_status->st_mode & S_IWGRP) ? 'w' : '-';
   perm[6] = (file_status->st_mode & S_IXGRP) ? ((file_status->st_mode & S_ISGID) ? 's' : 'x') : ((file_status->st_mode & S_ISGID) ? 'S' : '-');

   perm[7] = (file_status->st_mode & S_IROTH) ? 'r' : '-';
   perm[8] = (file_status->st_mode & S_IWOTH) ? 'w' : '-';
   perm[9] = (file_status->st_mode & S_IXOTH) ? ((file_status->st_mode & S_ISVTX) ? 't' : 'x') : ((file_status->st_mode & S_ISVTX) ? 'T' : '-');

   // Get modification time in ISO format
   char time_buf[20];
   strftime(time_buf, sizeof(time_buf), "%Y-%m-%d %H:%M",
            localtime(&file_status->st_mtime));

   // Handle symlink target
   char link_target[1024];
   int target_len = 0;
   if (perm[0] == 'l')
   {
      target_len = readlink(file_name, link_target, sizeof(link_target) - 1);
      if (target_len != -1)
      {
         link_target[target_len] = '\0';
      }
      else
      {
         strcpy(link_target, "???");
      }
   }

   // Modified output format
   printf("%s %lu %u %u %lld %s %s%s%s\n",
          perm,
          file_status->st_nlink,
          file_status->st_uid,
          file_status->st_gid,
          (long long)file_status->st_size,
          time_buf,
          file_name,
          (perm[0] == 'l') ? " -> " : "",
          (perm[0] == 'l') ? link_target : "");
}

// New comparison function for sorting
int compare_dirent(const void *a, const void *b)
{
   const struct dirent *da = *(const struct dirent **)a;
   const struct dirent *db = *(const struct dirent **)b;

   // Always show . and .. first
   if (strcmp(da->d_name, ".") == 0)
      return -1;
   if (strcmp(db->d_name, ".") == 0)
      return 1;
   if (strcmp(da->d_name, "..") == 0)
      return -1;
   if (strcmp(db->d_name, "..") == 0)
      return 1;

   return strcmp(da->d_name, db->d_name);
}

int main()
{
   DIR *dir = opendir(".");
   struct dirent **entries;
   int count;

   if (dir == NULL)
   {
      perror("Error opening directory");
      return EXIT_FAILURE;
   }

   // Read all directory entries into array
   count = scandir(".", &entries, NULL, compare_dirent);
   if (count == -1)
   {
      perror("Error reading directory");
      closedir(dir);
      return EXIT_FAILURE;
   }

   // Process each entry
   for (int i = 0; i < count; i++)
   {
      struct stat file_status;
      if (lstat(entries[i]->d_name, &file_status) == -1)
      {
         perror("Error reading file information");
         continue;
      }

      print_file_attributes(&file_status, entries[i]->d_name);
      free(entries[i]);
   }

   free(entries);
   closedir(dir);
   return EXIT_SUCCESS;
}