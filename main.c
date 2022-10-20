
#include <ncurses.h>
#include <string.h>
#include <stdlib.h>

#define HIGHT 30
#define WIDTH 70
#define row 9
#define col 60
#define HIT_PAIOR 1
#define BOMB_PAIR 2
#define BOTE1_PAIR 11
#define BOTE3_PAIR 13
#define BOTE4_PAIR 14
#define BOTE5_PAIR 15

void repaintBK();
void get_name_of_player(char **player_name, int player_name_pointer);
void get_format(char **entr, int pointer_helper);
int proccess_info(char**playerInfo);
int proces_coord_for_fire(char*fireCoord);
int curr_x = 13, curr_y = 0;
int bord_x = 1, bord_y=2;
int ch;
int win_cond = FALSE;
int esc_cond = FALSE;
int ship5 = 1, ship3 = 2, ship4 = 1, ship1 = 1;
enum player_status {PREP_STATE=0,GAME_STATE};
typedef enum player_status player_status_t;

int matrix[7][7]={0};


int main()
{
  //init local var
  char *player_name = (char *)malloc(20 * sizeof(char));
  int player_name_pointer = 0;
  char *entered_info = (char *)malloc(7 * sizeof(char));
  int pointer_helper = 0;
  player_status_t plStatus;
  plStatus=GAME_STATE;
  int printed1=0;

  start_color();
  init_pair(HIT_PAIOR,COLOR_RED,COLOR_BLACK);
  init_pair(BOMB_PAIR,COLOR_YELLOW,COLOR_BLACK);
  init_pair(BOTE1_PAIR,COLOR_BLUE,COLOR_BLACK);
  init_pair(BOTE3_PAIR,COLOR_MAGENTA,COLOR_BLACK);
  init_pair(BOTE4_PAIR,COLOR_GREEN,COLOR_BLACK);
  init_pair(BOTE5_PAIR,COLOR_WHITE,COLOR_BLACK);
  // init ncurses
  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  noecho();

  repaintBK(); // paint the bord with squares

  // GET USER INFO(username)
  attron(A_BOLD);
  printw("[HELLO PLAYER...]\n");
  curr_y = 21;
  curr_x = 10;
  printw("[WHAT IS YOUR NAME?]: ");
  attroff(A_BOLD);

  get_name_of_player(&player_name, player_name_pointer);
  curr_x+=3;
  attron(A_BOLD);
  printw("[SETTUP YOUR SHIPS CAPTIAN %s]\n", player_name);
  printw("[FORMAT]{ship_code-h/v-xcood-ycood}");
  attroff(A_BOLD);

  while (!win_cond || !esc_cond)
  {
    if(plStatus==PREP_STATE){
      mvprintw(curr_x, 0, "[CURRENT NUMBER OF SHIPS LEFT ");
      printw("{L5-ships:%d,L4-ships:%d,L3-ships:%d,L1-ships:%d}]\n", ship5, ship4, ship3, ship1);
      curr_x += 1;
      printw("[ENTER SHIP AND ITS COORDINATES IN FORMAT]:");
      ch = getch();
      if (ch == 27){
        esc_cond=true;
        break;
      }
      curr_y = 42;
      get_format(&entered_info, pointer_helper);
      curr_x -= 1;
      printw("[LASTE ENTERD INFO: %s]",entered_info);
      int changeStatus = proccess_info(&entered_info);
     
      if(changeStatus==1){
        plStatus=GAME_STATE;
        mvprintw(curr_x, 0, "[CURRENT NUMBER OF SHIPS LEFT ");
        printw("{L5-ships:%d,L4-ships:%d,L3-ships:%d,L1-ships:%d}]\n", ship5, ship4, ship3, ship1);
        }
    }else{
      if(printed1==0){
      mvprintw(11,0,"\n");
      mvprintw(12,0,"\n");
      mvprintw(13,0,"\n");
      mvprintw(14,0,"\n");
      mvprintw(15,0,"\n");
      move(11,0);
      printw("[READY TO PLAY CAPTAIN %s]\n",player_name);
      printw("[WHERE TO FIRE CAPTAIN ]: ");
      curr_x=12;
      curr_y=strlen("[WHERE TO FIRE CAPTAIN ]: ")-1;
      refresh();
      printed1=1;
      }
      
      
      refresh();
      
      ch=getch();
      char firePos[3];
      int pointerFire=0;
      while(ch!='\n') {
        if(pointerFire<3 && ch!=27 && ch!=KEY_BACKSPACE){
          addch((char)ch);
          firePos[pointerFire]=ch;
          pointerFire+=1;
           curr_y +=1;
           
           refresh();
        }
        ch=getch();

        if(ch==KEY_BACKSPACE){
          if(pointerFire==0)continue;
          pointerFire--;
          mvprintw(curr_x,curr_y," ");
          move(curr_x,curr_y);
          curr_y -=1;
          
           refresh();
        }
          if(ch==27)break;
         
      }

          move(curr_x,curr_y-2);
          printw("\n");
          move(curr_x,curr_y-2);
          refresh();
          proces_coord_for_fire(firePos);

        break;
      
    }

  }

  while (ch != 27)
  {
    ch = getch();
  }
  endwin();

  return 0;
}

int proces_coord_for_fire(char*fireCoord){

        int x=0,y=0;
        char*helper;
        char ch_helper;
      if(strlen(fireCoord)==0)return -1;
      if(strlen(fireCoord)==1)return -3;
      if(strlen(fireCoord)==2){
            if(fireCoord[0]==' ')return -2;
            if(fireCoord[0]!=' ' && fireCoord[1]==' ')return -2;
            for(int i=0;i<2;i++){
              if(fireCoord[i]<'0'|| fireCoord[i]>'9')return -4;
            }
            ch_helper=fireCoord[0];
            helper=&(ch_helper);
            x= atoi(helper);
            ch_helper=fireCoord[1];
            helper=&(ch_helper);
            y= atoi(helper);
            attron(COLOR_PAIR(HIT_PAIOR));
            printw("hello world");
            attroff(COLOR_PAIR(HIT_PAIOR));
            refresh();
      }
      else
       if(strlen(fireCoord)==3){

      }


}










int check_valid_ship_name(char*ship_name,int*shipId);
int check_h_v(char *hv,int*in);
int check_coo_input(int x , int y);
void input_player_choois(int id,int vh,int x,int y); 


int proccess_info(char**playerInfo){
    
  char separator[2]="-";
  char * token;
  int vh;
  int shipID=0;
  token = strtok(*playerInfo,separator);
  if(token==NULL)return 0; // if player entered nothing

  if(check_valid_ship_name(token,&shipID)==0)return 0; // L5 L4 L3 L1
  token = strtok(NULL, separator);
  
if(token==NULL)return 0;
  if(check_h_v(token,&vh)==0)return 0 ;// h or v
 


  token =  strtok(NULL,separator);
  if(token==NULL)return 0;
  int en_x = atoi(token);
  token =  strtok(NULL,separator);
 
  if(token==NULL)return 0;
  int en_y = atoi(token);
 
  if( check_coo_input(en_x,en_y)==0)return 0;// if x[1-7] and y[1-7]
 
  input_player_choois(shipID,vh,en_x,en_y);
  if(ship1==0)return 1;
return 0;
}

int check_valid_ship_name(char*ship_name,int* shipId){

  if(strcmp(ship_name,"L5")==0){
    if(ship5==0){
    *shipId=0;
    printw("[ERROR YOU DON'T HAVE ENOUGH OF SHIP L5]\n");
    
    }else
    *shipId=5;
    return 1;
  }else
  if(strcmp(ship_name,"L4")==0){
     if(ship4==0){
    *shipId=0;
    printw("[ERROR YOU DON'T HAVE ENOUGH OF SHIP L4]\n");
    
    }else
     *shipId=4;
 return 1;
  }else
  if(strcmp(ship_name,"L3")==0){
     if(ship3==0){
    *shipId=0;
    printw("[ERROR YOU DON'T HAVE ENOUGH OF SHIP L3]\n");
    
    }else
     *shipId=3;
 return 1;
  }else
  if(strcmp(ship_name,"L1")==0){
     if(ship1==0){
    *shipId=0;
    printw("[ERROR YOU DON'T HAVE ENOUGH OF SHIP L1]\n");
    
    }else
     *shipId=1;
 return 1;
  }

  else return 0;
}

int check_h_v(char *hv,int* in){
  if(strcmp(hv,"h")==0){
  *in=1;
 return 1;
  }else
  if(strcmp(hv,"v")==0){
  *in=2;
  return 1;
  }
  else return 0;
}
int check_coo_input(int x , int y){
 

  if(x>7 || x<1)return 0;
  if(y>7 || y<1)return 0;
    
  return 1;
}

void input_player_choois(int id,int vh,int x,int y){
  if(id==0)return;
  //cses:
  if(vh==1 && id!=1 && x==1 && y>1)return;
  if(vh==2 && id!=1 && x==1 && y==7)return;
  if(vh==2 && id!=1 && x==7 && y==7)return;

  if(vh==1 && id==3 && x<3 )return;
  if(vh==1 && id==4 && x<4 )return;
  if(vh==1 && id==5 && x<5 )return;

  if(vh==2 && id==3 && y>5)return;
  if(vh==2 && id==4 && y>4)return;
  if(vh==2 && id==5 && y>3)return;
 
  int status=0;
  
  
  for(int i =0 ;i<id;i++){
    if(vh==1){
        if(matrix[x-1-i][y-1]==1){status=1; break;}
    }else{
        if(matrix[x][y-1+i]==1){status=1; break;}
    }
  }
  if(status==0){
     for(int i =0 ;i<id;i++){
        if(vh==1){
            matrix[x-1+i][y-1]=1;
            mvaddch(x-i,y*4-2,'@');
        }else{
            matrix[x][y-1+i]=1;
            mvaddch(x,(y+i)*4-2,'@');
        }
      }
      if(id==5)ship5-=1;
      if(id==4)ship4-=1;
      if(id==3)ship3-=1;
      if(id==1)ship1-=1;

      
  }

}




void get_format(char **entr, int pointer_helper)
{
  int cur_y_temp= curr_y;
  char entered_info[9];
  while (ch != '\n')
  {
    if (ch != KEY_BACKSPACE)
    {

      if (pointer_helper < 8)
      {
        addch((char)ch);
        entered_info[pointer_helper] = ch;
        pointer_helper++;
        curr_y += 1;
      }
    }

    ch = getch();

    if (ch == '\n')
      addch('\n');
    if (ch == KEY_BACKSPACE)
    {
      if (curr_y != 42)
      {
        if (pointer_helper != 0)
          pointer_helper -= 1;

        mvprintw(curr_x, curr_y, " ");

        move(curr_x, curr_y);
        curr_y -= 1;
        refresh();
      }
    }
  }
  
  
  strcpy(*entr, entered_info);
  
   mvprintw(curr_x,cur_y_temp,"            \n");
  
}

void get_name_of_player(char **player_name, int player_name_pointer)
{
  ch = getch();
  char temp[20];
  while (ch != '\n')
  {
    if (ch != KEY_BACKSPACE)
    {
      if (player_name_pointer < 19)
      {
        addch((char)ch);
        temp[player_name_pointer] = (char)ch;
        player_name_pointer += 1;
        curr_y += 1;
      }
    }
      ch = getch();
      if (ch == KEY_BACKSPACE)
      {
        if (curr_y != 21)
        {
          if (player_name_pointer != 0)
            player_name_pointer -= 1;

          mvprintw(curr_x, curr_y, " ");

          move(curr_x, curr_y);
          curr_y -= 1;
          refresh();
        }
      }
    
  }
  temp[player_name_pointer] = '\0';
  addch('\n');
  strcpy(*player_name, temp);
}

char matrix_map[]="--1---2---3---4---5---6---7-----1---2---3---4---5---6---7--\n|___|___|___|___|___|___|___|#|___|___|___|___|___|___|___|1\n|___|___|___|___|___|___|___|#|___|___|___|___|___|___|___|2\n|___|___|___|___|___|___|___|#|___|___|___|___|___|___|___|3\n|___|___|___|___|___|___|___|#|___|___|___|___|___|___|___|4\n|___|___|___|___|___|___|___|#|___|___|___|___|___|___|___|5\n|___|___|___|___|___|___|___|#|___|___|___|___|___|___|___|6\n|___|___|___|___|___|___|___|#|___|___|___|___|___|___|___|7\n--1---2---3---4---5---6---7-----1---2---3---4---5---6---7--\n";

void repaintBK()
{
  printw(matrix_map);
}
