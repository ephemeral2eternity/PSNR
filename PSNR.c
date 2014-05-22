#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

//#define INPUT1 "D:\\exp\\brave_best.yuv"
//#define INPUT2 "D:\\exp\\brave-02-32-02-rec.yuv"


#define uint8_t unsigned char
float psnr(uint8_t* p1, uint8_t* p2, int size);
void mReturn(void);
uint8_t * buf1, *buf2;
FILE* in_file1, *in_file2, *fp;

int main(int argc, char* argv[])
{
	
    int frame_no;
	int totalFrmNo;
	int size, luma_size, chroma_size;
	int height,width;
	float y = 0.0, u = 0.0, v = 0.0;
	float perY = 0.0;
	buf1=NULL;
	buf2=NULL;
	if (argc!=6) 
	{
		printf("invalid input parameter!\n\n");
		printf("help: Format example\nPSNR.exe  file1.yuv  file2.yuv  height  width frameNum\n");
		return 0;
	}
	    
	/*
	{ 
		//#define VIDEOMODE CIF
		height = 288;
		width = 352;
	}
	else
	{
		height = 576;
		width = 704;
	}
	*/
	height = atoi(argv[3]);
	width = atoi(argv[4]);
	totalFrmNo = atoi(argv[5]);

	//height = 800;
	//width = 1920;
	luma_size = height * width;
    size = luma_size + (luma_size >> 1);	
	chroma_size = luma_size / 4;

    buf1 = (uint8_t *)malloc(size);
    buf2 = (uint8_t *)malloc(size);

	in_file1 = fopen(argv[1], "rb");
    if (!in_file1)
    {
        printf("cannot open input file %s\n", argv[1]);
        mReturn();
    }
	in_file2 = fopen(argv[2], "rb");
    if (!in_file2)
    {
        printf("cannot open input file  %s\n", argv[2]);
        mReturn();
    }

    fp = fopen("psnr.txt", "w");

for(frame_no = 0; frame_no < totalFrmNo; frame_no++ )
{
	if (fread(buf1, size, 1, in_file1) <= 0)
	{	
		printf("cannot read from input file.");
		break;
	}

	if (fread(buf2, size, 1, in_file2) <= 0)
	{
		printf("cannot read from input file.");
		break;
	}

	y += psnr(buf1, buf2, luma_size);
	perY = psnr(buf1, buf2, luma_size);
	fprintf(fp, "%.4f \n", perY);
	printf("Frame %d has the psnr %.4f (db) \n", frame_no, perY);


	u += psnr(buf1 + luma_size, buf2 + luma_size, chroma_size);
	v += psnr(buf1 + luma_size + chroma_size, buf2 + luma_size + chroma_size, chroma_size);
	
}

	printf("total frames: %d \n", frame_no);
	printf(" The luma psnr is: %.4f \n", y/frame_no);
	printf(" The chroma_u psnr is: %.4f \n", u/frame_no);
	printf(" The chroma_v psnr is: %.4f \n", v/frame_no);

    fclose(fp);
	return 0;
}

void mReturn(void)
{
	if(!in_file1) fclose(in_file1);
	if(!in_file2) fclose(in_file2);
	if(!buf1) free(buf1);
	if(!buf2) free(buf2);
}
float psnr(uint8_t* p1, uint8_t* p2, int size)
{
    unsigned long sad = 0;
    int i;
	
    for (i = 0 ; i < size ; i ++)
    {
        int tmp;
        tmp = (p1[i] - p2[i]);
        sad += tmp * tmp;
    }

    return (float)(10 * log10(65536.0f * size/sad));
}