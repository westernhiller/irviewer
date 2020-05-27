#ifndef _IRX_H
#define _IRX_H

typedef struct
{
    char tag[8];    // 'i', 'r', 'x' + 5 '\0'
    int  width;     // image width
    int  height;    // image height
    int  bytes;     // = 2
    int  frames;    // number of image frames
    char reserved[32];  // reserved for extra file information
} IRXHEADER;

#endif // _IRX_H
