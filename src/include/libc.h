#define TRUE 1
#define FALSE 0
const char dec[11] = "0123456789";
const char hex[17] = "0123456789ABCDEF";

int pow(int a, int b) {
    int r = 1;
    for (int i = 0; i < b; i++) r *= a;
    return r;
}
char Equal(char *s1, char *s2, int lenth) {
    char res = 1;
    for (int i = 0; i < lenth; i++) if(s1[i] != s2[i]) res = 0;
    return res;
}
int StrLenth(char *str) {
    int r = 0;
    while (str[r] != 0) r++;
    return r;
}
void w2str(char*str, word n) {
    int strl = 1;
    str[0] = n%10 + '0';
    if(n/10!=0) {
        str[1] = (n/10)%10 + '0';
        strl++;
    }
    else str[1] = 0;
    if(n/100!=0)  {
        str[2] = (n/100)%10 + '0';
        strl++;
    }
    else str[2] = 0;
    if(n/1000!=0)  {
        str[3] = (n/1000)%10 + '0';
        strl++;
    }
    else str[3] = 0;
    str[4] = 0;
    rstr(str, strl-1);
    if(strl > 3) {
        char buff;
        buff = str[1];
        str[1] = str[2];
        str[2] = buff;
    }
}
void rstr(char *str, int len) {
    char buff;
    for (int i = 0; i < len; i++) {
        buff = str[i];
        str[i] = str[len-i];
        str[len-i] = buff;
    }
}
void MemFill(char *str, char c, int l) {
    for (int i = 0; i < l; i++) str[i] = c;
}