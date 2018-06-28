#include <iostream>
#include <sstream>
#include <windows.h>

using namespace std;

// wypisz czyja kolej
void pokazCzyjaTura(bool gracz)
{
    if(gracz)
    cout<<"            TURA CZARNYCH (x)"<<endl<<endl;
    else
    cout<<"            TURA BIALYCH (o)"<<endl<<endl;
}

// inicjalizacja wstepnego ustawienia pionkow na planszy
void przygotujPlansze(char (&plansza)[8][8])
{
    //zabawy z petlami i instrukcjami warunkowymi zeby uzyskac odpowiednie ustawienie pionkow
    //moglbys rownie dobrze recznie powpisywac jak ma wygladac ustawienie jako tablica charow
    //petle skracaja robote

    for(int i=0;i<8;i++)
    for(int j=0;j<8;j++)
    plansza[i][j]=' ';

    for(int i=0;i<3;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(j%2==1 && i!=1)
            plansza[i][j]='x';
            else
            {
                if(i==1)
                if(j%2==0)
                plansza[i][j]='x';
            }
        }
    }
    for(int i=5;i<8;i++)
    {
        for(int j=i-5;j<8;j++)
        {
            if(i==7 && j==2)
            plansza[i][j-2]='o';
            plansza[i][j]='o';
            j++;
        }
    }
}

//rysowanie planszy
void rysuj(char plansza[8][8])
{
    //jak powyzej tylko zamiast ustawiania pionkow rysujemy cala plansze

    int tx=0,ty=-1;
    char x='A';
    cout<<"   ";
    for(int i=0;i<8;i++)
    {
        cout<<"  "<<x<<" ";
        x++;
    }
    cout<<endl;
    int y=8;
    for(int i=0;i<17;i++)
    {
        cout<<" ";
        if(i%2!=0)
        {
            cout<<y<<" ";
            y--;
            ty+=1;
        }
        else
        cout<<"  ";
        for(int j=0;j<9;j++)
        {
            if(j==8)
            {
                if(i%2==0)
                cout<<"+";
                else
                cout<<"|";
            }
            else
            {
                if(i%2==0)
                cout<<"+---";
                else
                {
                    cout<<"| "<<plansza[ty][tx]<<" ";
                    tx++;
                    if(tx==8)
                    tx=0;
                }
            }
        }
        cout<<endl;
    }
    cout<<endl;
}

//Zmiana tury, jesli gracz ktory wykonal ruch byl bialy, nastepny jest gracz czarny i odwrotnie

bool nastepnyGracz(bool gracz)
{
    if(gracz)
    return false;
    else
    return true;
}

//sprawdzenie poprawnosc polecenia, zwrocenie wartosci wspolrzednych w tablicy
string sprawdzPolecenie(string polecenie)
{
    //sprawdzam czy dlugosc polecenia jest rowna 2 (x,y)
    if(polecenie.length()!=2)
    {
        return "failure";
    }
    else
    {
        int x=0;
        bool posiadaCyfre=false; // zmienna odpowiadajaca za sprawdzenie czy w
        //naszym poleceniu o dlugosci 2 znajduje sie tez cyfra

        //tutaj odwoluje sie od ASCII https://www.asciitable.com/
        //sprawdzam czy pierwszy znak jest cyfra w przedziale od 1 do 8 (ASCII 1=49, 8=56)
        if(polecenie[0]>=49 && polecenie[0]<=56)
        {
            posiadaCyfre=true;
            x=1;
            polecenie[0]=56-polecenie[0]+48;
        }
        else
        //jesli pierwszy znak nie byl cyfra to sprawdzamy drugi, na tej samej zasadzie
        if(polecenie[1]>=49 && polecenie[1]<=56)
        {
            posiadaCyfre=true;
            x=0;
            polecenie[1]=56-polecenie[1]+48;
        }
        //jesli wykrylismy juz cyfre to musimy sprawdzic czy drugi z kolei znak jest litera od a do h
        //bierzemy pod uwage male i duze litery (ASCII a=97, h=104, A=65, H=72)
        if(posiadaCyfre)
        {
            if((polecenie[x]>=65 && polecenie[x]<=72) || (polecenie[x]>=97 && polecenie[x]<=104))
            {
                //zamiana znakow ASCII na odpowiednie liczby, beda nam potrzebne do znalezienia
                //miejsca w tablicy
                if(polecenie[x]>=65 && polecenie[x]<=72)
                polecenie[x]-=17;
                else
                polecenie[x]-=49;
                //jesli znaki byly odwrotnie ulozone (a3/3a) to zmieniamy je tak zeby bez problemu odczytac pozycje w tablicy
                //na zasadzie wspolrzednych x,y
                if(x==0)
                {
                    char temp=polecenie[0];
                    polecenie[0]=polecenie[1];
                    polecenie[1]=temp;
                }
                return polecenie;
            }
            else
            return "failure";
        }
        else
        return "failure";
    }
}

// pobierz mozliwe ruchy dla danego pionka
string pobierzMozliweRuchy(int x,int y, char plansza[8][8], bool gracz)
{
    string ruchy="";
    stringstream ss;
    //stringstream czyli potok/strumien sluzy do przekonwertowania dowolnej wartosci na string
    //tutaj wykorzystamy liczby w postaci wspolrzednych x y aby zapisac je jako mozliwe ruchy do lancucha znakow string
    char pionek;
    if(!gracz)
    pionek='x';
    else
    pionek='o';
    //opisze tylko jedna instrukcje warunkowa, reszta dziala na identycznej zasadzie
    //w warcabach pionki poruszaja sie po skosie, sprawdzamy wiec pierwszy skos
    //[?] [ ] [ ]
    //[ ] [x] [ ]
    //[ ] [ ] [ ]
    //x - pionek, ? sprawdzane pole
    //jesli wspolrzedne x to (0,0), punkt ? bedzie mial wspolrzedne (-1,1), w tablicy zapisuje odwrotnie x,y - plansza[y][x]
    //pierwszy if to sprawdzenie czy punkt ktory sprawdzamy nie wychodzi poza tablice, rozmiar naszej tablicy to 8
    //indeksy numeruja sie od 0 do 7
    //jesli x lub y sa wieksze od 7 lub mniejsze od 0 - znajduja sie poza plansza (poza pamiecia)
    if(((y+1)<=7) && ((x-1)>=0))
    {
        //sprawdzam czy miejsce jest puste
        if(plansza[y+1][x-1]==' ')
        {
            ss<<(y+1)<<(x-1);
            ruchy+=ss.str()+" ";
            ss.str("");
            //dodaje wspolrzedne punktu ? do listy mozliwych ruchow
            //czyszcze strumien
        }
        //jesli miejsce nie jest puste to sprawdzam czy znajduje sie tam pionek przeciwnika
        else if(plansza[y+1][x-1]==pionek)
        {
            //[?] [ ] [ ] [ ] [ ]
            //[ ] [o] [ ] [ ] [ ]
            //[ ] [ ] [x] [ ] [ ]
            //[ ] [ ] [ ] [ ] [ ]
            //[ ] [ ] [ ] [ ] [ ]
            // x- pionek, o - przeciwnik, ? punkt w ktorym wyladujemy
            //jesli tak, sprawdzam czy moge go "zbic" poprzez wyladowanie tuz za nim:
            //sprawdzam czy punkt za nim miesci sie w obszarze planszy/pamieci
            if(((y+2)<=7) && ((x-2)>=0))
            {
                //jesli tak to sprawdzam czy jest pusty
                if(plansza[y+2][x-2]==' ')
                {
                    //dodajemy do listy mozliwych ruchow, czyscimy potok stringa
                    ss<<(y+2)<<(x-2);
                    ruchy+=ss.str()+" ";
                    ss.str("");
                }
            }
        }
    }
    if(((y-1)>0) && ((x-1)>=0))
    {
        if(plansza[y-1][x-1]==' ')
        {
            ss<<(y-1)<<(x-1);
            ruchy+=ss.str()+" ";
            ss.str("");
        }
        else if(plansza[y-1][x-1]==pionek)
        {
            if(((y-2)>=0) && ((x-2)>=0))
            {
                if(plansza[y-2][x-2]==' ')
                {
                    ss<<(y-2)<<(x-2);
                    ruchy+=ss.str()+" ";
                    ss.str("");
                }
            }
        }
    }
    if(((y+1)<=7) && ((x+1)<=7))
    {
        if(plansza[y+1][x+1]==' ')
        {
            ss<<(y+1)<<(x+1);
            ruchy+=ss.str()+" ";
            ss.str("");
        }
        else if(plansza[y+1][x+1]==pionek)
        {
            if(((y+2)<=7) && ((x+2)<=7))
            {
                if(plansza[y+2][x+2]==' ')
                {
                    ss<<(y+2)<<(x+2);
                    ruchy+=ss.str()+" ";
                    ss.str("");
                }
            }
        }
    }
    if(((y-1)>0) && ((x+1)<=7))
    {
        if(plansza[y-1][x+1]==' ')
        {
            ss<<(y-1)<<(x+1);
            ruchy+=ss.str()+" ";
            ss.str("");
        }
        else if(plansza[y-1][x+1]==pionek)
        {
            if(((y-2)>=0) && ((x+2)<=7))
            {
                if(plansza[y-2][x+2]==' ')
                {
                    ss<<(y-2)<<(x+2);
                    ruchy+=ss.str()+" ";
                    ss.str("");
                }
            }
        }
    }
    if(ruchy=="")
    ruchy="Brak mozliwego ruchu dla wybranego pionka!";
    return ruchy;
}

// Sprawdzamy czy istnieje w tym miejscu nasz pionek i czy moze wykonac ruch
// Jesli tak, ta funkcja zwroci liste mozliwych do wykonania ruchow, jesli nie, odpowiedni komunikat
string wybierzPionka (string polecenie, char plansza[8][8], bool gracz)
{
    int x,y;
    //zmienna temp to tymczasowe zmienne sluzace jako pudelko na dane, nie ma zadnej konkretnej nazwy ani
    //glebszego zastosowania, czasem po prostu zwracam tempa bo nie mam pomyslu na nazwe albo nie chce
    //inicjowac kolejnej zmiennej, mozesz nazwac ja ruch
    string temp="";
    temp+=polecenie[0];
    //atoi(string.c_str()) to funkcja ktora zamienia danego stringa/lancuch znakow na inta, wartosc liczbowa
    y=atoi(temp.c_str());
    temp="";
    temp+=polecenie[1];
    x=atoi(temp.c_str());
    temp="";
    char pionek;
    if(!gracz)
    pionek='o';
    else
    pionek='x';
    //obliczamy wspolrzedne w oparciu o otrzymane polecenie, nastepnie sprawdzamy czy mamy tam dostepnego pionka
    //zaleznego od aktualnego gracza (jestem bialymi, sprawdzam czy tam jest pionek "o")
    if(plansza[y][x]!=pionek)
    temp="Brak dostepnego pionka na wybranym polu!";
    else
    //jesli mamy pionka, pobieramy jego mozliwe ruchy
    temp=pobierzMozliweRuchy(x,y,plansza,gracz);
    return temp;
}

//sprawdzamy czy polecenie ktore otrzymalismy znajduje sie na
//liscie mozliwych ruchow wybranego przez nas pionka
bool czyMozeWykonacRuch(string polecenie, string ruchy)
{
    bool temp=false;
    size_t pos=ruchy.find(polecenie);
    if(pos!=string::npos)
    temp=true;
    //te 3 linijki sprawdzaja czy dany string zawiera sie w innym stringu
    //na przyklad czy "Ala" zawiera sie w "Ala ma kota"
    return temp;
}

//funkcja ta nie zawiera zadnej kontroli bledow, inne funkcje zostaly wykorzystane do tego celu
//funkcja ta wykonuje ruch:
//usuwa pionek z poprzedniej pozycji, rysuje go w nowej pozycji, jesli pionek przeciwnika zostal zbity,
//kasuje pionek przeciwnika i redukuje ilosc pionkow o 1
void wykonajRuch(string polecenie,string polecenie2,bool gracz, char (&plansza)[8][8],int &czarne, int &biale)
{
    string temp_x="";
    string temp_y="";
    temp_y+=polecenie[0];
    temp_x+=polecenie[1];
    int temp_iy1=atoi(temp_y.c_str());
    int temp_ix1=atoi(temp_x.c_str());
    //tradycyjna zamiana lancuchow znakow na liczby
    plansza[temp_iy1][temp_ix1]=' ';
    //usuwanie pionka ze starej pozycji
    if(abs(polecenie[0]-polecenie2[0])==2)
    {
        //sprawdzamy czy pionek jest bity (wartosc bezwzgledna roznicy pozycji)
        if(!gracz)
        czarne--;
        else
        biale--;
        temp_x="";
        temp_y="";
        temp_y+=polecenie2[0];
        temp_x+=polecenie2[1];
        int temp_iy2=atoi(temp_y.c_str());
        int temp_ix2=atoi(temp_x.c_str());
        temp_iy2=temp_iy1-temp_iy2;
        temp_ix2=temp_ix1-temp_ix2;
        //mamy podane dwie pozycje, szukamy tej pomiedzy zeby usunac z niej pionek przeciwnika (reprezentacje graficzna)
        if(temp_iy2>0)
        {
            temp_iy2-=1;
        }
        else
        temp_iy2+=1;
        if(temp_ix2>0)
        {
            temp_ix2-=1;
        }
        else
        temp_ix2+=1;
        temp_iy1-=temp_iy2;
        temp_ix1-=temp_ix2;
        plansza[temp_iy1][temp_ix1]=' ';
        //usuwamy zbijany pionek;
    }
    char pionek;
    if(!gracz)
    pionek='o';
    else
    pionek='x';
    temp_x="";
    temp_y="";
    temp_y+=polecenie2[0];
    temp_x+=polecenie2[1];
    temp_iy1=atoi(temp_y.c_str());
    temp_ix1=atoi(temp_x.c_str());
    plansza[temp_iy1][temp_ix1]=pionek;
    //rysowanie pionka w nowej pozycji
}

int main()
{
    char plansza[8][8];
    string bledy[2];
    bledy[0]="Brak dostepnego pionka na wybranym polu!";
    bledy[1]="Brak mozliwego ruchu dla wybranego pionka!";
    int biale=12,czarne=12; // pionki
    bool gracz=0; //0 = zaczynaja biale
    string polecenie,polecenie2;
    string info="";
    przygotujPlansze(plansza);
    while(czarne!=0 && biale!=0)
    {
        system("cls"); //czyszczenie ekranu
        bool restart=false; //czy zostal wykonany ruch
        pokazCzyjaTura(gracz);
        rysuj(plansza);
        cout<<info<<endl; //wypisanie komunikatu bledu (jest pusty jesli nie ma zadnego bledu)
        info="";
        cin>>polecenie;
        polecenie=sprawdzPolecenie(polecenie);
        if(polecenie=="failure")
        info="Niepoprawne polecenie!";
        else
        {
            info=wybierzPionka(polecenie,plansza,gracz); //zwraca wszystkie dostepne ruchy dla pionka albo blad
            //zwiazany z ich brakiem lub nieprawidlowym pionkiem
            for(int i=0;i<2;i++)
            {
                if(info==bledy[i])
                {
                    restart=true;
                    break;
                }
            }
            if(!restart)
            {
                cin>>polecenie2;
                polecenie2=sprawdzPolecenie(polecenie2);
                if(polecenie2=="failure")
                {
                    info="Niepoprawne polecenie!";
                    restart=true;
                }
                else
                {
                    if(!czyMozeWykonacRuch(polecenie2,info))
                    {
                        info="Ruch niedostepny!";
                        restart=true;
                    }
                    else
                    {
                        wykonajRuch(polecenie,polecenie2,gracz,plansza,czarne,biale);
                        info="";
                    }
                }
            }
        }
        if(!restart)
        {
            gracz=nastepnyGracz(gracz);
        }
    }
    system("cls");
    if(czarne>0)
    cout<<"WYGRANA CZARNYCH!"<<endl;
    else
    cout<<"WYGRANA BIALYCH"<<endl;
    return 0;
}
