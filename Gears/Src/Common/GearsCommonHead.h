/************************************************************************/
/* Author:  Kemi Peng 
   History: 08/03/2015 by Kemi Peng*/
/************************************************************************/

#ifndef GEARSCOMMONHEAD
#define GEARSCOMMONHEAD

#include <string>
#include <stdio.h>
#include <assert.h>
#ifdef WIN32
#include "windows.h"
#endif

#ifndef _DEBUG
#include "mmgr.h"
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p!=NULL) { delete (p);     (p)=NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p!=NULL) { delete[] (p);   (p)=NULL; } }
#endif    
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p!=NULL) { (p)->Release(); (p)=NULL; } }
#endif


#define ALERT(a) {char ____moji[1024];sprintf_s(____moji,"%s\nIgnore£¿",a);if (MessageBox(NULL, ____moji , "ALERT" , MB_YESNO) != IDYES) *(int*)(NULL) = 0;}

#ifdef _DEBUG
inline static void HR( bool isRight ) 
{
	if ( !isRight )
	{
		ALERT("Error");
	}
}
#else
inline extern void HR( bool isRight ) 
{
	return;
}
#endif


#ifdef _DEBUG
static const char *logFile = ".\\Resources\\gears.log";
static inline void Log(const char *format, ...)
{

	// Build the buffer

	static char buffer[2048];
	va_list	ap;
	va_start(ap, format);
	vsprintf_s(buffer, format, ap);
	va_end(ap);

	// Open the log file
	FILE	*fp = NULL;
	fopen_s( &fp, logFile, "ab");
	// If you hit this assert, then the memory logger is unable to log information to a file (can't open the file for some
	// reason.) You can interrogate the variable 'buffer' to see what was supposed to be logged (but won't be.)
	HR( fp!=NULL );

	if (!fp) return;

	// Spit out the data to the log

	fprintf(fp, "%s\r\n", buffer);
	fclose(fp);
};

// also write to log
static inline void DEBUG_PRINT( const char* format, ... )
{
	static char buffer[1024];
	va_list	ap;
	va_start(ap, format);
	vsprintf_s(buffer, format, ap);
	va_end(ap);

	printf( "%s\r\n", buffer );
	Log( buffer );
};
#else
static inline void DEBUG_PRINT( const char* format, ... )		
{
	/*static char buffer[1024];
	va_list	ap;
	va_start(ap, format);
	vsprintf_s(buffer, format, ap);
	va_end(ap);
	Log( buffer );*/
}
#endif

// load binary file
static void *LoadBinaryStream(const char *filename, unsigned int *size)
{
	FILE *file = fopen(filename, "rb");
	if ( file==NULL )
		return NULL;

	unsigned int file_size = 0;

	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	fseek(file, 0, SEEK_SET);
	unsigned char *buffer = new unsigned char[file_size];

	if ( buffer==NULL )
	{
		fclose(file);
		return NULL;
	}

	fread(buffer, file_size, 1, file);
	fclose(file);

	if ( size ) *size = file_size;

	return buffer;
}

// find out the dirctory of a file
// for example, if src is "c:\path\dest.file", then dest is "c:\path\"
static void GetDirFromFile(char *dest, const char *src)
{
	int idx=strlen(src)-1;
	for (; idx>=0; idx--)
	{
		if(src[idx] == '/' || src[idx] == '\'')
		{
			break;
		}
	}

	int n = 0;
	while(n<=idx)
	{
		dest[n] = src[n];
		n++;
	}

	dest[n] = '\0';

}
#endif