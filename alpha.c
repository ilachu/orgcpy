#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "stdlib.h"
#include "sys/types.h"
#include "dirent.h"
#include "regex.h"
#include "sys/wait.h"

//global declarations
char dir[50][50],cp_arg1[50];
int dir_count,flag;
regex_t mp3,flac,video,exe,img,other;
//edit to change default destinations
char *music_dest="/home/lachu/Desktop/music_",
     *image_dest="/home/lachu/Desktop/images_",
     *dev_dest="/home/lachu/Desktop/dev_",
     *video_dest="/home/lachu/Desktop/videos_",
     *other_dest="/home/lachu/Desktop/others_";

//prototypes
void exec_cp(int index,int flag);

//function to split the dir list string into an array
//takes string list, returns dir count
int split(char string[]){
    int len=strlen(string),j=0,i=0,k=0;
    while (i<len)
    {
    	if (string[i]=='\n')
    	{
    		dir[j][k]='\0';
    		j++;
    		i++;
    		k=0;
    	}
      	dir[j][k]=string[i];
    	i++;
    	k++;
    }
    return j+1;
}
//compile regular expressions
void compile_regex(){
	regcomp(&mp3,".mp3$",0);
	regcomp(&flac,".flac$",0);
	regcomp(&video,".mp4$",0);
	regcomp(&exe,"[exe,rb,html,c,cpp]$",0);
	regcomp(&img,"[jpg,bmp,img]$",0);
	regcomp(&other,".*",0);
	printf("%s\n","regular expressions compiled");
}
//match and copy to resp dir
void org_cp(int index){
	if (!(regexec(&mp3,dir[index],0,NULL,0)))
	{
		printf("[Audio]copying...\n");
		exec_cp(index,0);
	}
	else if(!(regexec(&flac,dir[index],0,NULL,0)))
	{
		printf("[Audio]copying...\n");
		exec_cp(index,0);
	}
	else if(!(regexec(&video,dir[index],0,NULL,0))){
		printf("[video]copying...\n");
		exec_cp(index,1);
	}
	else if (!(regexec(&exe,dir[index],0,NULL,0)))
	{
		printf("[Dev file]copying...\n");
		exec_cp(index,2);
	}
	else if(!(regexec(&img,dir[index],0,NULL,0))) {
		printf("[image]copying...\n");
		exec_cp(index,3);

	}
	else if(!(regexec(&other,dir[index],0,NULL,0))){
		printf("[other]...\n");
		exec_cp(index,4);
	}

}
void exec_cp(int index,int flag){
    char command[100];
	char *dest;
	if (flag==0)
	{
		dest=music_dest;
	}
	else if (flag==1)
	{
		dest=video_dest;
	}
	else if (flag==2)
	{
		dest=dev_dest;
	}
	else if (flag==3)
	{
		dest=image_dest;
	}
	else {
		dest=other_dest;
	}

	//if destination dir doesnt exist
	//checks and creates one
	sprintf(command,"test -d \"%s\" || mkdir -p \"%s\"",dest,dest);
	printf("checking if directory exists...\n");
	system(command);
    
    //build and execute cp command
    sprintf(command,"cp %s %s",dir[index],dest);
	printf("executing command \"%s\"\n",command);
	system(command);

	printf("copied to destination: \"%s\"\n",dest);
}
int main(int argc, char *argv[])
{
	//dir count and list initialize
	dir_count=split(argv[1]);

    //traverse directories	
	DIR *check_dir;
	compile_regex();
	for (int i = 0; i < dir_count; ++i)
	{
		printf("%s",dir[i]);
		check_dir=opendir(dir[i]);
		if (check_dir==NULL) //if it's a file
		{
			printf("%s\n"," not a directory...");
			org_cp(i);
		}
		else { // in case of dir
			printf(" direcotry opened.\n");
			char command[100];
			sprintf(command,"orgcpy %s/%s",argv[2],dir[i]);
			printf("command :::\"%s\"\n",command );
			system(command);
			printf("......[%s orgcpy'ed].......\n",dir[i]);

			}	
	}
	
	printf("%s\n",argv[2]);
    return 0;
}
