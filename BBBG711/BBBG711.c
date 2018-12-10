
#include "stdafx.h"

void print_usage(char *program_name)
{
    printf("Usage: %s input_file CONVERSION output_file\n", program_name);
    printf("Supported CONVERSIONs: pcm_alaw, pcm_ulaw, alaw_pcm, ulaw_pcm\n");
}

long get_file_size(FILE *f)
{
    long file_size;

    /* Go to end of file */
    fseek(f, 0L, SEEK_END);

    /* Get the number of bytes */
    file_size = ftell(f);

    /* reset the file position indicator to 
    the beginning of the file */
    fseek(f, 0L, SEEK_SET);	

    return file_size;
}

char * allocate_buffer(long buffer_size)
{
    char *buffer;

    /* grab sufficient memory for the 
    buffer to hold the audio */
    buffer = (char*)calloc(buffer_size, sizeof(char));	
    /* memory error */
    if(buffer == NULL)
    {
        printf("Error while allocating memory for write buffer.\n");
        exit(EXIT_FAILURE);
    }

    return buffer;
}

struct PCMheader {
	uint8_t RIFF[4];
	uint32_t FileSize;
	uint8_t WAVEfmt[8];
	uint32_t fmtSize;
	uint16_t formattag;
	uint16_t nChannels;
	uint32_t frequency;
	uint32_t bytes_per_second;
	uint16_t bytes_by_capture;
	uint16_t bits_per_sample;
	uint8_t w_data[4];
	uint32_t bytes_in_data;
};

void printPCMheader(struct PCMheader header)
{
	int i = 0;
	printf("\n\nPCM header\n");
	printf("header size: %d\n",sizeof(header));
	printf("RIFF: %c%c%c%c\n",header.RIFF[0],header.RIFF[1],header.RIFF[2],header.RIFF[3]);
	printf("File Size: %d\n",header.FileSize);
	printf("WAVEfmt :");
	for(i=0;i<8;i++)
	{
		printf("%c",header.WAVEfmt[i]);
	}
	printf("\nformat chunk size: %d\n",header.fmtSize);
	printf("Audio format tag: %d\n",header.formattag);
	printf("channels: %d\n",header.nChannels);
	printf("sample rate: %d samples/second\n",header.frequency);
	printf("byte rate: %d bytes/second\n",header.bytes_per_second);
	printf("bytes by capture: %d\n",header.bytes_by_capture);
	printf("bits per sample: %d\n",header.bits_per_sample);
	printf("data: %c%c%c%c\n",header.w_data[0],header.w_data[1],header.w_data[2],header.w_data[3]);
	printf("bytes in data: %d\n",header.bytes_in_data);
}

void initPCMheader(struct PCMheader * header)
{
	header->RIFF[0]='R';
	header->RIFF[1]='I';
	header->RIFF[2]='F';
	header->RIFF[3]='F';
	header->WAVEfmt[0]='W';
	header->WAVEfmt[1]='A';
	header->WAVEfmt[2]='V';
	header->WAVEfmt[3]='E';
	header->WAVEfmt[4]='f';
	header->WAVEfmt[5]='m';
	header->WAVEfmt[6]='t';
	header->WAVEfmt[7]=' ';
	header->w_data[0]='d';
	header->w_data[1]='a';
	header->w_data[2]='t';
	header->w_data[3]='a';
	header->fmtSize = 16;
	header->formattag = 1;
	header->bytes_by_capture = 2;
	header->bits_per_sample = 16;
	return;
}

struct G711header {
	uint8_t RIFF[4];
	uint32_t FileSize;
	uint8_t WAVEfmt[8];
	uint32_t fmtSize;
	uint16_t formattag;
	uint16_t nChannels;
	uint32_t frequency;
	uint32_t bytes_per_second;
	uint16_t blockAlign;
	uint16_t bits_per_sample;
	uint16_t cbSize;
	uint8_t fact[4];
	uint16_t cfSizeLSB;
	uint16_t cfSizeMSB;
	uint16_t sampleLengthLSB;
	uint16_t sampleLengthMSB;
	uint8_t w_data[4];
	uint16_t dataLengthLSB;
	uint16_t dataLengthMSB;
	uint16_t blockAlign2;
};

void printG711header(struct G711header header)
{
	int i = 0;
	printf("\n\nG711 header\n");
	printf("header size: %d\n",sizeof(header));
	printf("RIFF: %c%c%c%c\n",header.RIFF[0],header.RIFF[1],header.RIFF[2],header.RIFF[3]);
	printf("File Size: %d\n",header.FileSize);
	printf("WAVEfmt :");
	for(i=0;i<8;i++)
	{
		printf("%c",header.WAVEfmt[i]);
	}
	printf("\nformat chunk size: %d\n",header.fmtSize);
	printf("format tag: %d\n",header.formattag);
	printf("channels: %d\n",header.nChannels);
	printf("sample rate: %d samples/second\n",header.frequency);
	printf("byte rate: %d bytes/second\n",header.bytes_per_second);
	printf("block align: %d\n",header.blockAlign);
	printf("bits per sample: %d\n",header.bits_per_sample);
	printf("extension size (cbSize): %d\n",header.cbSize);
	printf("fact: %c%c%c%c\n",header.fact[0],header.fact[1],header.fact[2],header.fact[3]);
	printf("fact block size LSB: %d\n",header.cfSizeLSB);
	printf("fact block size MSB: %u\n",header.cfSizeMSB);
	printf("sample length LSB: %u\n",header.sampleLengthLSB);
	printf("sample length MSB: %u\n",header.sampleLengthMSB);
	printf("data: %c%c%c%c\n",header.w_data[0],header.w_data[1],header.w_data[2],header.w_data[3]);
	printf("data length LSB: %d\n",header.dataLengthLSB);
	printf("data length MSB: %d\n",header.dataLengthMSB);
	printf("block align: %d\n\n",header.blockAlign2);
}
void initG711header(struct G711header * header)
{
	header->RIFF[0]='R';
	header->RIFF[1]='I';
	header->RIFF[2]='F';
	header->RIFF[3]='F';
	header->WAVEfmt[0]='W';
	header->WAVEfmt[1]='A';
	header->WAVEfmt[2]='V';
	header->WAVEfmt[3]='E';
	header->WAVEfmt[4]='f';
	header->WAVEfmt[5]='m';
	header->WAVEfmt[6]='t';
	header->WAVEfmt[7]=' ';
	header->fact[0]='f';
	header->fact[1]='a';
	header->fact[2]='c';
	header->fact[3]='t';
	header->w_data[0]='d';
	header->w_data[1]='a';
	header->w_data[2]='t';
	header->w_data[3]='a';
	header->fmtSize = 18;
	header->blockAlign = 1;
	header->bits_per_sample = 8;
	header->cbSize = 0;
	header->cfSizeLSB = 4;
	header->cfSizeMSB = 0;
	header->blockAlign2 = 0xFFFF;
	return;
}

int wmain(int argc, wchar_t *argv[])
{
    FILE    *fRead, *fWrite;
    char    *bufferRead, *bufferWrite;
    long    bufferReadSize, bufferWriteSize;
    size_t  readed;
	struct PCMheader pcm_header;
	struct G711header g711_header;
	uint32_t bytes_in_data = 0;


    if(argc != 4)
    {
        printf("Incorrect parameter length.\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    /* open an existing file for reading */
	printf("opening %s\n",argv[1]);
    fRead = _wfopen(argv[1],L"rb");

    /* quit if the file does not exist */
    if( fRead == NULL )
    {
       printf("Error while opening read file.\n");
       exit(EXIT_FAILURE);
    }
	/* open a file for writing */
    fWrite = _wfopen(argv[3], L"wb");

    /* quit if the file can not be opened */
    if( fWrite == NULL )
    {
       printf("Error while opening the write file.\n");
       exit(EXIT_FAILURE);
    }
    /* Conversions */

    if (strcmp(argv[2], L"g711_pcm") == 0) 
    {
		// Read in header
		fread(&g711_header,1,sizeof(g711_header),fRead);
		/* Get file size */
		printG711header(g711_header);
		bufferReadSize = g711_header.FileSize - sizeof(g711_header);
		printf("bufferReadSize: %d\n",bufferReadSize);
		/* grab sufficient memory for the buffer to hold the audio */
		bufferRead = allocate_buffer(bufferReadSize);
		/* copy all the data into the buffer */
		readed = fread(bufferRead, sizeof(char), bufferReadSize, fRead);
		printf("bytes read in: %d\n",readed);
		if (readed != bufferReadSize)
		{
			printf("Incorrect bytes read in\n");
			exit(EXIT_FAILURE);
		}
		fclose(fRead);
		initPCMheader(&pcm_header);
		pcm_header.nChannels = g711_header.nChannels;
		pcm_header.frequency = g711_header.frequency;
		pcm_header.bytes_per_second = g711_header.bytes_per_second*2;
		pcm_header.bytes_in_data = g711_header.FileSize - sizeof(g711_header);
		pcm_header.bytes_in_data *= 2;
		pcm_header.FileSize = sizeof(pcm_header)+pcm_header.bytes_in_data;
		printPCMheader(pcm_header);
		fwrite (&pcm_header , 1, sizeof(pcm_header), fWrite);
		if(g711_header.formattag == 7)
		{
			printf("Converting mu-law to pcm\n");
			ulaw_pcm16_tableinit();
			bufferWriteSize = bufferReadSize * 2;
			bufferWrite = allocate_buffer(bufferWriteSize);
			ulaw_to_pcm16(bufferReadSize, bufferRead, bufferWrite);

		}
		else if(g711_header.formattag == 6)
		{
			printf("converting A-law to pcm\n");
			alaw_pcm16_tableinit();
			bufferWriteSize = bufferReadSize * 2;
			bufferWrite = allocate_buffer(bufferWriteSize);
			alaw_to_pcm16(bufferReadSize, bufferRead, bufferWrite);
		}
		else 
		{
			printf("Input file is not G711 encoded.\n");
			exit(EXIT_FAILURE);
		}
		
        
    } 
    else if (strcmp(argv[2], L"pcm_alaw") == 0)
    {
		// Read in header
		fread(&pcm_header,1,sizeof(pcm_header),fRead);
		// Print bytes of data field
		printPCMheader(pcm_header);
		/* Get file size */
		bufferReadSize = pcm_header.bytes_in_data;
		/* grab sufficient memory for the buffer to hold the audio */
		bufferRead = allocate_buffer(bufferReadSize);
		/* copy all the text into the buffer */
		readed = fread(bufferRead, sizeof(char), bufferReadSize, fRead);
		if (readed != bufferReadSize)
		{
			printf("Incorrect bytes readed\n");
			exit(EXIT_FAILURE);
		}
		fclose(fRead);
		initG711header(&g711_header);
		g711_header.formattag = 6;
		g711_header.nChannels = pcm_header.nChannels;
		g711_header.frequency = pcm_header.frequency;
		g711_header.bytes_per_second = pcm_header.bytes_per_second/pcm_header.bytes_by_capture;
		g711_header.sampleLengthLSB = (pcm_header.bytes_in_data/2)&0x0000FFFF;
		g711_header.sampleLengthMSB = ((pcm_header.bytes_in_data/2)>>16)&0x0000FFFF;
		g711_header.dataLengthLSB = g711_header.sampleLengthLSB;
		g711_header.dataLengthMSB = g711_header.sampleLengthMSB;
		g711_header.FileSize=sizeof(g711_header)+(pcm_header.bytes_in_data/2);
		printG711header(g711_header);
		fwrite (&g711_header , 1, sizeof(g711_header), fWrite);
        pcm16_alaw_tableinit();
        bufferWriteSize = bufferReadSize / 2;
        bufferWrite = allocate_buffer(bufferWriteSize);
        pcm16_to_alaw(bufferReadSize, bufferRead, bufferWrite);
    }
    else if (strcmp(argv[2], L"pcm_ulaw") == 0)
    {
		// Read in header
		fread(&pcm_header,1,sizeof(pcm_header),fRead);
		// Print bytes of data field
		printPCMheader(pcm_header);
		/* Get file size */
		bufferReadSize = pcm_header.bytes_in_data;
		/* grab sufficient memory for the buffer to hold the audio */
		bufferRead = allocate_buffer(bufferReadSize);
		/* copy all the text into the buffer */
		readed = fread(bufferRead, sizeof(char), bufferReadSize, fRead);
		if (readed != bufferReadSize)
		{
			printf("Incorrect bytes readed\n");
			exit(EXIT_FAILURE);
		}
		fclose(fRead);initG711header(&g711_header);
		g711_header.formattag = 7;
		g711_header.nChannels = pcm_header.nChannels;
		g711_header.frequency = pcm_header.frequency;
		g711_header.bytes_per_second = pcm_header.bytes_per_second/pcm_header.bytes_by_capture;
		g711_header.sampleLengthLSB = (pcm_header.bytes_in_data/2)&0x0000FFFF;
		g711_header.sampleLengthMSB = ((pcm_header.bytes_in_data/2)>>16)&0x0000FFFF;
		g711_header.dataLengthLSB = g711_header.sampleLengthLSB;
		g711_header.dataLengthMSB = g711_header.sampleLengthMSB;
		g711_header.FileSize=sizeof(g711_header)+(pcm_header.bytes_in_data/2);
		printG711header(g711_header);
		fwrite (&g711_header , 1, sizeof(g711_header), fWrite);initG711header(&g711_header);
        pcm16_ulaw_tableinit();
        bufferWriteSize = bufferReadSize / 2;
        bufferWrite = allocate_buffer(bufferWriteSize);
        pcm16_to_ulaw(bufferReadSize, bufferRead, bufferWrite);
    }
    else
    {
        printf("Incorrect parameter.\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    printf("Bytes read: %ld, Buffer Write: %ld\n", bufferReadSize, bufferWriteSize);
    /* copy all the buffer into the file */
    fwrite (bufferWrite , sizeof(char), bufferWriteSize, fWrite);
    fclose (fWrite);

    /* free the memory we used for the buffer */
    free(bufferWrite);
    free(bufferRead);

    return 0;
}