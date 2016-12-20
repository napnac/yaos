#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <kernel/ramdisk.h>
#include <kernel/tar.h>
#include <kernel/vfs.h>

void test_ramdisk(void)
{
   extern Vfs_node *root;
   extern Vfs_node *ramdisk_files;
   extern uint32_t nb_ramdisk_files;
   extern Tar_entry *init_entry;

   for(size_t i = 0; i < nb_ramdisk_files; ++i) {
      printf("Found '%s': ", ramdisk_files[i].name);
      if(ramdisk_files[i].type == DIRECTORY_TYPE)
         printf("(directory)");
      else if(ramdisk_files[i].type == FILE_TYPE) {
         printf("(file, %d bytes)", ramdisk_files[i].size);

         Tar_entry *file = tar_get_entry(init_entry, i);
         char buf[256];
         char *content = ((char *) file) + TAR_ENTRY_SIZE;

         memcpy(buf, content, ramdisk_files[i].size);
         buf[ramdisk_files[i].size] = '\0';
         printf("\nContent: %s", buf);
      }
      else
         printf("Error: Unknown file type '%d'\n", ramdisk_files[i].type);

      printf("\n");
   }

   Vfs_node *test_read_dir = vfs_read_dir(root, 1);
   printf("test read dir: %s\n", test_read_dir->name);

   Vfs_node *test_find_dir = vfs_find_dir(root, "usr/test_dir/test_subfile");
   printf("test find dir: %s\n", test_find_dir->name);
}
