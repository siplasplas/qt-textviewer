//
// Created by Andrzej Borucki on 2022-09-13
//

#include <cstdio>
#include <random>
#include <cstring>
#include <algorithm>

void generateBigLine() {
    const char *s = "0123456789";
    int count = 1000*1000;
    FILE *f;
    f = fopen("bigline.dat","wb");
    for (int i=0; i<count; i++) {
        fwrite(s, 10, 1, f);
    }
    fclose(f);
}

void generateBigAtEnd() {
    int count = 1000*1000;
    int maxLen = 10;
    FILE *f;
    f = fopen("bigfile.dat","wb");
    for(int part = 0; part<6; part++) {
        char *buf=new char[maxLen+2];
        for (int i=0; i<count; i++) {
            size_t actualLen = random()%maxLen;
            char aux[32];
            sprintf(aux, "%c%d ",'a'+part,i);
            int l = (int)std::min(actualLen,strlen(aux));
            strncpy(buf, aux, l);
            int pos = l;
            while (pos<actualLen) {
                char c = (pos % 26)+'a';
                buf[pos]=c;
                pos++;
            }
            switch(rand()%3){
                case 0:
                    buf[pos]='\n';
                    pos++;
                    break;
                case 1:
                    buf[pos]='\r';
                    pos++;
                    break;
                case 2:
                    buf[pos]='\r';
                    pos++;
                    buf[pos]='\n';
                    pos++;
                    break;
            }
            fwrite(buf, pos, 1, f);
        }
        delete[] buf;
        count /= 10;
        maxLen *= 10;
    }
    fclose(f);
}

void generateBigAtStart() {
    int count = 10;
    int maxLen = 1000*1000;
    FILE *f;
    f = fopen("bigfile3.dat","wb");
    for(int part = 0; part<6; part++) {
        char *buf=new char[maxLen+2];
        for (int i=0; i<count; i++) {
            size_t actualLen = random()%maxLen;
            char aux[32];
            sprintf(aux, "%c%d ",'a'+part,i);
            int l = (int)std::min(actualLen,strlen(aux));
            strncpy(buf, aux, l);
            int pos = l;
            while (pos<actualLen) {
                char c = (pos % 26)+'a';
                buf[pos]=c;
                pos++;
            }
            switch(rand()%3){
                case 0:
                    buf[pos]='\n';
                    pos++;
                    break;
                case 1:
                    buf[pos]='\r';
                    pos++;
                    break;
                case 2:
                    buf[pos]='\r';
                    pos++;
                    buf[pos]='\n';
                    pos++;
                    break;
            }
            fwrite(buf, pos, 1, f);
        }
        delete[] buf;
        count *= 10;
        maxLen /= 10;
    }
    fclose(f);
}

int main() {

}