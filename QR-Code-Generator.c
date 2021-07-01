#include <stdio.h>

void gen_QR (char (*colour));  //Generate QR  Image with chosen color
void printQR ( unsigned char hash_ch[] , char (*colour), int decider );  //Print the required QR
void print_element_24 (unsigned char ch, int index, char (*colour), char *QR[20][40]);  //Identify the correct place for given character in small QR image
void print_element_132 (unsigned char ch, int index, char (*colour), char *QR[38][76]);  //Identify the correct place for given character in large QR image
void chr_int (unsigned char ch, int dec[]); //Assign ASCI value as binary number to an integer array
int lenStr (char arry[]);  //To get the length of char array
void hashString (char array[] , int input_len, unsigned char hashStr[], int decider);  //Hashed the user input string
void place_print_24 (int id, char *QR[20][40], char (*colour), int row_st, int char_bina[]);  //Print character in correct position [20]
void place_print_132 (int id, char *QR[38][76], char (*colour), int row_st, int char_bina[]);  //print character in correct position [132]
int str_int (char str[]);  //Convert string to an integer
void down (char str_arry[], char down_array[]);  //Convert string in to lowercase

int main(int argc, char *argv[]) {

    char *colour;
    char flag_h[] = "-h";
    char flag_c[] = "-c";
    char str_lower[100] = {};

    void usage_print(){  //Function to print usage of the program when needed
        printf("usage :\n");
        printf("%s -c [black|red|green|yellow|blue|magenta|cyan]\n", argv[0]);
        printf("%s -h for the help about the program\n", argv[0]);
    }

    //Decision making with Flags (Argument Handling)
    if ( (argc == 3) && (str_int(argv[1]) == str_int(flag_c)) ){
	down(argv[2], str_lower);
        if (str_int(str_lower) == str_int("black")){
            colour = "\033[;40m \033[0;m"; //Black
            gen_QR(colour);
        }else if (str_int(str_lower) == str_int("red")){
            colour = "\033[;41m \033[0;m";  //Red
            gen_QR(colour);
        }else if (str_int(str_lower) == str_int("green")){
            colour = "\033[;42m \033[0;m";  //Green
            gen_QR(colour);
        }else if (str_int(str_lower) == str_int("yellow")){
            colour = "\033[;43m \033[0;m";  //Yellow
            gen_QR(colour);
        }else if (str_int(str_lower) == str_int("blue")){
            colour = "\033[;44m \033[0;m";  //Blue
            gen_QR(colour);
        }else if (str_int(str_lower) == str_int("magenta")){
            colour = "\033[;45m \033[0;m";  //Magenta
            gen_QR(colour);
        }else if (str_int(str_lower) == str_int("cyan")){
            colour = "\033[;46m \033[0;m";  //Cyan
            gen_QR(colour);
        }else {
            printf("Invalid argument for color.\n");
            usage_print();
        }
    }else if ((argc ==2) &&  ( str_int(argv[1]) == str_int(flag_h))){
        usage_print(); //Print usage of program
    }else if (argc == 1){
        colour = "\033[;40m \033[0;m";  //When color is not mentioned print the QR in Black-White
        gen_QR(colour);
    }else {
        printf("Incorrect usage of arguments.\n");
        usage_print();
    }return 0;
}

void gen_QR (char (*colour)){ //Generate the QR image

    char input[121]={};
    unsigned char hash[133]={} ;
    int input_len;

    printf("Enter the URL: ");
    scanf("%s", input);  //Scan the input string
    input_len = lenStr(input);

    if (input_len <=3){
        printf("String is too short. Not supported by available QR versions\n");
    }else if (input_len <20){  //QR printing for 24 length Hashed String
        printf("\e[1;1H\e[2J");
        hashString(input, input_len, hash, 24);
        printQR(hash, colour, 24);
    }else if (input_len >=20 || input_len < 120){  //QR printing for 132 length Hashed String
        printf("\e[1;1H\e[2J");
        hashString(input, input_len, hash, 132);
        printQR(hash, colour, 132);
    }else if (input_len >= 120){
    	printf("String is too long. Not supported by available QR versions\n");
    }
}

void printQR ( unsigned char hash_ch[] , char (*colour), int decider ){ //Printing function to Print QR image in terminal
    //According to the hash string initialize the size of QR image
    int widthQR=0, lengthQR=0;
    if (decider == 24){
        lengthQR = 20;
        widthQR = 40;
    }else if (decider == 132){
        lengthQR = 38;
        widthQR = 76;
    }
    char *ch[lengthQR][widthQR];
    //Create the basic frame for QR according to length of hashed String
    for(int i=0; i<lengthQR; i++){
        for(int j=0; j<widthQR; j++){
            ch[i][j] = "\033[;47m \033[0;m";
        }
    }
    //Identify the Identification Squares and add them
    for (int i=1; i<(lengthQR-1); i++){
        for(int j=2; j<(widthQR-2); j++){
            if ( ((( (j>5) && (j<10) ) || ( (j>widthQR-11) && (j<widthQR-6))) && ((i==3) || (i==4))) || (((j>5) && (j<10)) && ((i==lengthQR-4) || (i==lengthQR-5))) ) {
                ch[i][j] = colour;
            }else if ( (((j>widthQR-15) || (j<14)) && ((i==1) || (i==6))) || ((j<14) && ((i==lengthQR-7) || (i==lengthQR-2))) ){
                ch[i][j] = colour;
            }else if ( (((i>1) && (i<6)) && ((j<4) || (j>widthQR-5) || (j==12) || (j==13) || (j==widthQR-14) || (j==widthQR-13))) || (((i>lengthQR-7) && (i<lengthQR-2)) && ((j<4) || (j==12) || (j==13))) ) {
                ch[i][j] = colour;
            }
        }
    }
    //Send character to the print_element_function to print each characters in QR image
    if (decider == 24){
        for (int i=0; i<decider; i++){
            print_element_24(hash_ch[i], (i+1), colour, ch);
        }
    }else if (decider == 132){
        for (int i=0; i<decider; i++){
            print_element_132(hash_ch[i], (i+1), colour, ch);
        }
    }
    //Print the QR image in Terminal
    for (int i=0; i<lengthQR; i++){
        for (int j=0; j<widthQR; j++){
            printf("%s", ch[i][j]);
        }printf("\n");
    }
    printf("\n");
}

void print_element_24 (unsigned char ch, int index, char (*colour), char *QR[20][40]){ //Function to identify position in small QR image

    int chr_bina [9]={};
    chr_int(ch, chr_bina); //Convert character in to binary number

    //Identify the correct positions of characters in 24 length Hashed string in small QR Image
    if ((index<=4) || ((index>20) && (index<=22))){
        int j;
        if (index <= 4){
            j = ((index -1)*6) + 14;
        }else{
            j = ((index -21)*6) + 2;
        }place_print_24(j, QR, colour, 7, chr_bina);
    }else if (((index>4) && (index<=8)) || ((index>22) && (index<=24))){
        int j;
        if (index <= 8){
            j = ((index -5)*6) + 14;
        }else{
            j = ((index -23)*6) + 2;
        }place_print_24(j, QR, colour, 10, chr_bina);
    }else if ((index>8) && (index<=12)){
        int j = ((index -9)*6) + 14;
        place_print_24(j, QR, colour, 13, chr_bina);
    }else if ((index>12) && (index<=16)){
        int j = ((index -13)*6) + 14;
        place_print_24(j, QR, colour, 16, chr_bina);
    }else if ((index>16) && (index<=18)){
        int j = ((index -17)*6) + 14;
        place_print_24(j, QR, colour, 1, chr_bina);
    }else if ((index>18) && (index<=20)){
        int j = ((index -19)*6) + 14;
        place_print_24(j, QR, colour, 4, chr_bina);
    }
}

void print_element_132 (unsigned char ch, int index, char (*colour), char *QR[38][76]){  ////Function to identify position in large QR image

    int chr_bina [9]={};
    chr_int(ch, chr_bina);  //Convert character in to binary number

    //Identify the correct positions of characters in 132 length Hashed string in large QR Image
    if ((index<=10) || ((index>116) && (index<=118))){
        int j;
        if (index <= 10){
            j = ((index -1)*6) + 14;
        }else{
            j = ((index -117)*6) + 2;
        }place_print_132(j, QR, colour, 7, chr_bina);
    }else if (((index>10) && (index<=20)) || ((index>118) && (index<=120))){
        int j;
        if (index <= 20){
            j = ((index -11)*6) + 14;
        }else{
            j = ((index -119)*6) + 2;
        }place_print_132(j, QR, colour, 10, chr_bina);
    }else if (((index>20) && (index<=30)) || ((index>120) && (index<=122))){
        int j;
        if (index <= 30){
            j = ((index -21)*6) + 14;
        }else{
            j = ((index -121)*6) + 2;
        }place_print_132(j, QR, colour, 13, chr_bina);
    }else if (((index>30) && (index<=40)) || ((index>122) && (index<=124))){
        int j;
        if (index <= 40){
            j = ((index -31)*6) + 14;
        }else{
            j = ((index -123)*6) + 2;
        }place_print_132(j, QR, colour, 16, chr_bina);
    }else if (((index>40) && (index<=50)) || ((index>124) && (index<=126))){
        int j;
        if (index <= 50){
            j = ((index -41)*6) + 14;
        }else{
            j = ((index -125)*6) + 2;
        }place_print_132(j, QR, colour, 19, chr_bina);
    }else if (((index>50) && (index<=60)) || ((index>126) && (index<=128))){
        int j;
        if (index <= 60){
            j = ((index -51)*6) + 14;
        }else{
            j = ((index -127)*6) + 2;
        }place_print_132(j, QR, colour, 22, chr_bina);
    }else if (((index>60) && (index<=70)) || ((index>128) && (index<=130))){
        int j;
        if (index <= 70){
            j = ((index -61)*6) + 14;
        }else{
            j = ((index -129)*6) + 2;
        }place_print_132(j, QR, colour, 25, chr_bina);
    }else if (((index>70) && (index<=80)) || ((index>130) && (index<=132))){
        int j;
        if (index <= 80){
            j = ((index -71)*6) + 14;
        }else{
            j = ((index -131)*6) + 2;
        }place_print_132(j, QR, colour, 28, chr_bina);
    }else if ((index>80) && (index<=90)){
        int j = ((index -81)*6) + 14;
        place_print_132(j, QR, colour, 31, chr_bina);
    }else if ((index>90) && (index<=100)){
        int j = ((index -91)*6) + 14;
        place_print_132(j, QR, colour, 34, chr_bina);
    }else if ((index>100) && (index<=108)){
        int j = ((index -101)*6) + 14;
        place_print_132(j, QR, colour, 1, chr_bina);
    }else if ((index>108) && (index<=116)){
        int j = ((index -109)*6) + 14;
        place_print_132(j, QR, colour, 4, chr_bina);
    }
}

void chr_int (unsigned char ch, int dec[]){  //Convert character in to binary number and assign to integer array
    int count = ch;
    for (int i=0, j=8; i<9; i++, --j){
        if ((count%2) == 0){
            dec[j]=0;
        }else {
            dec[j]=1;
        }count = count/2;
    }
}

int lenStr (char arry[]){  //Return the length of a string
    int len =0;
    for (int i=0; arry[i] !=0; i++){
        len++;
    }return len;
}

void hashString(char array[], int input_len, unsigned char hashStr[], int decider){ //Making hashed string for input link
    char str_1;
    str_1 = 50 + input_len;
    hashStr[0] =str_1;

    for (int a  =1; a <=input_len; a++){
        hashStr[a] = array[a-1];
    }int c = decider-1, b =1, f =1, g =1;
    for (; b <= (((decider -1) -input_len)/input_len) ; b++){
        for (int e =0; (c > (decider-1)-(input_len*b)) && (e < input_len); c--,e++ ){
            hashStr[c] = array[e] + f;
        }f++;
    }g = c;
    for (int h =0;(g > (c-(((decider -1)-input_len)%input_len)) && (h < input_len)); g--,h++){
        hashStr[g] = array[h]+f;
    }
}

void place_print_132 (int id, char *QR[38][76], char (*colour), int row_st, int char_bina[]){  //Printing characters in exact position in large QR Image
    int ind=0;
    for(int i=row_st,k=id; i<(row_st+3); i++){
        for(; k<(id+6); k=k+2){
            if( char_bina[ind]==1){
                QR[i][k] = colour;
                QR[i][k+1] = colour;
            }ind++;
        }k=id;
    }
}

void place_print_24 (int id, char *QR[20][40], char (*colour), int row_st, int char_bina[]){  //Printing characters in exact position in small QR Image
    int ind=0;
    for(int i=row_st,k=id; i<(row_st+3); i++){
        for(; k<(id+6); k=k+2){
            if( char_bina[ind]==1){
                QR[i][k] = colour;
                QR[i][k+1] = colour;
            }ind++;
        }k=id;
   }
}

int str_int (char str[]){  //Convert character in to 9-length binary number and add to number array
    int j=0;
    for (int i=0; i<lenStr(str); i++){
        j= j + str[i];
    }return j;
}

void down (char str_arry[], char down_array[]){  //Convert alphabetic character in to lowercase letter
    for (int i=0; i<lenStr(str_arry); i++){
        if (str_arry[i] <90){
            down_array[i] = str_arry[i] + 32;
        }else {
            down_array[i] = str_arry[i];
        }
    }
}
