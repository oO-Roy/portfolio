#ifndef FILE_SYSTEM_PARSING_H
#define FILE_SYSTEM_PARSING_H

#include <stddef.h> /* For size_t */

/******************************************************************************/
/*  
    This API is created on the concept of device '/dev/ram0' with the FS 
    (file-system) of type 'ext2', this kind of device can be achived by:
    
    """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
    "   :    mkdir -p /home/XXX/github/ext2                               "
    "   :    sudo modprobe brd rd_nr=1 rd_size=1050000 max_part=0      "
    "   :    sudo mkfs.ext2 /dev/ram0                                  "
    "   :    sudo mount -t ext2 -o sync /dev/ram0 /home/XXX/github/ext2   "
    "   :    sudo chown XXX /home/XXX/github/ext2                         "
    "   :    sudo chown XXX /dev/ram0                                  "
    """"""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""""
                                                                              */
/******************************************************************************/
/* -----------------------------------------------------------------------------
    @ description:      opens the file specified by file_name.
                        The return value is an inode that is an index to an 
                        entry in the inode table.
    @ params:           device_name:    name of the virtual disc.
                        file_name:      path to a spesific file
   ------------------------------------------------------------------------------ */
int EXT2Open(const char* device_name, const char* file_name);


    
    /* -----------------------------------------------------------------------------
    @ description:      attempts to read up to count bytes from file descriptor fd into
                        the buffer starting at buf.
                        
    @return:            On  success,  the number of bytes read, On  error,  -1.
    
    @ params:           fd:   an inode returnd from EXT2Open().
                        buf: a buffer to read into.
                        count: the number of byts to read;
    */
int EXT2Read(int target_inode, void* buf, size_t count);


#endif /* FILE_SYSTEM_PARSING_H */