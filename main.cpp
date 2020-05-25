#include <iostream>
#include "plant.h"
#include <cmath>
#include <discpp.h>
using namespace std;

void Vvod(int &L, int &N, double &min, double &max, double &d, int issl, double &sec)
{
    cout << "Введите канал опроса: ";
    cin >> L;
    cout << "Введите число контролируемых единиц продукции: ";
    cin >> N;
    switch (issl)
    {
        case 0:  {cout << "Bmin: ";
            cin >> min;
            cout << "Bmax: ";
            cin >> max;
            cout << "dB: ";
            cin >> d;
            cout << "Введите номинальное значений переменной X: ";
            cin >> sec;
            break;
        }
        case 1:  {
            cout << "Xmin: ";
            cin >> min;
            cout << "Xmax: ";
            cin >> max;
            cout << "dX: ";
            cin >> d;
            cout << "Введите допустимое отклонение от номинала переменной X: ";
            cin >> sec;
            break;
        }
    }
}

void kontrolizm( double B,double X[],double Xn,int i,int &narush)
{
    if (B>fabs(X[i]-Xn) && -B<fabs(X[i]-Xn))
        narush=0;
    else {
        narush=1;
    }
}

void vivod(double B[], double Xn[], double pers[], int issl, double min, double max, double d, int size) {
    // Объект для рисования графика
    Dislin g;

    g.metafl("cons");

    g.scrmod("reverse");

    g.disini();

    switch (issl) {
        case 0: g.name("B", "x");
            break;
        case 1: g.name("Xn", "x");
            break;
    }

    g.name("% of defect", "y");


    const int background_color = g.intrgb(0.95, 0.95, 0.95);

    g.axsbgd(background_color);

    g.ticks(10, "x");
    g.ticks(10, "y");


    // Задает координатную сетку.
    g.graf(
            // Область значений по оси абсцисс (X).
            min, max,
            // Наименьшее значение на оси абсцисс и следующее за ним.
            min, d,
            // Область значений по оси ординат (Y).
            0, 100,
            // Наименьшее значение на оси ординат и следующее за ним.
            0, 10);

    g.color("green");

    switch (issl) {
        case 0: g.curve(&B[0], &pers[0], size);
            break;
        case 1: g.curve(&Xn[0], &pers[0], size);
            break;
    }

    g.disfin();
}

int main()
{
    int  L, N, i,j,narush, issl;
    double B[1000], Xn[1000],X[1000],pers[1000], min, max, d, now, count, sec;
    Plant plant;
    plant_init(plant);
    cout << "Выберите, от чего будет производиться исследование, от B(0) или Xn(1):";
    cin >> issl;
    Vvod(L, N, min, max, d, issl, sec);
    now = min;
    cout << endl << endl;
    switch (issl)
    {
        case 0:{
            cout << "№           B           %брака" << endl;
            B[0] = now;
            Xn[0] = sec;
            break;
        }
        case 1:{
            cout << "№           X           %брака" << endl;
            Xn[0] = now;
            B[0] = sec;
            break;
        }
    }

    i = 0;
    while (now <= max)
    {
        count=0;
        for(j=0; j<N;j++)
        {
            X[j] = plant_measure(L, plant);

            switch (issl) {
                case 0: kontrolizm(B[i],X,Xn[0],j,narush);
                    break;
                case 1: kontrolizm(B[0],X,Xn[i],j,narush);
                    break;
            }

            if (narush==1){
                count+=1;
            }
        }
        pers[i]=(count/(N))*100;
        cout << i+1 << "          "<< now << "          " << pers[i] << "%" << endl;
        now+=d;
        i+=1;
        switch (issl) {
            case 0: B[i]=now;
                break;
            case 1: Xn[i]=now;
                break;
        }
    }

    vivod(B, Xn, pers, issl, min, max, d, i);

    return 0;
}