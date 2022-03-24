char* getcommand(char* line);
char* getfilename(char* line);

void main() {
  char line[80];
  char* filename;
  char* command;
  char buffer[13312];
  char ch[1];
  while(1){
    interrupt(0x21, 0, "Shell> \0", 0, 0);
    interrupt(0x21, 0x01, line, 0, 0);

    //get the command
    command = getcommand(line, command);

    //get the file name
    filename = getfilename(line, filename);
    interrupt(0x21, 0, "\r\n\0", 0, 0);

    if(compareCommand(command, "type\0")){	
      //interrupt to read file
      interrupt(0x21, 0x03, filename, buffer, 0);
      //print out the file
      interrupt(0x21, 0, buffer, 0, 0);

      /*currently, our type part does not work for file not found. It is still printing the message from before (from the file that was found) because the buffer stores the message from the previous cycle. We need to fix it so that the buffer is updated to an empty string and reloaded in each cycle.*/
      
    }else if(compareCommand(command, "execute\0")){
      //interrupt to execute file
      interrupt(0x21, 0x04, filename, 0x2000, 0);

      /*currently, it prints out junk and not the message from the user programs. It printed out the same junk when called in a loop*/
      
    }else{
      interrupt(0x21, 0, "Unrecognized command\r\n\0", 0, 0);
    }
    
  }
}

char* getcommand(char* line, char* command) {
  int i;
  
  while (line[i] != ' ') { //try to get the command name before the space
    command[i] = line[i];
    i++;
  }
  command[i] = '\0';
  return command;
}

char* getfilename(char* line,  char* filename) {
  int i=0;
  int j=0;
  
  while (line[i] != ' ') { //try to reach the space then read the file name 
    i++;
  }
  i++; //skip the space character
  
  while (line[i] != '\0') { //read the file name from the characters after the space
    filename[j] = line[i];
    j++;
    i++;
  }
  
  filename[j] = '\0';
  return filename;
}

int compareCommand(char* cmd1, char* cmd2){
  while(*cmd1 != '\0' && *cmd2 != '\0'){
    if(*cmd1 != *cmd2){ //commands not same
      return -1;
    }
    *cmd1++;
    *cmd2++;
  }
  return 1; //command is the same
}
