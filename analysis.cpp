#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <set>
using namespace std;

enum class StateUD
{
    Solved   = 0,
    Bars     = 1,
    Slashes  = 2,
    BarSlash = 3,
};

enum class StateFB
{
    Solved   = 0,
    OneFace = 1,
    Bars     = 2,
    OneBar   = 3,
};

enum class State
{
    Slashes_OneFace               =  0,
    Slashes_Bars                  =  1,
    Solved_Solved                 =  2,
    Solved_OneFace_Same           =  3,
    Solved_OneFace_Different      =  4,
    Solved_Bars                   =  5,
    Solved_OneBar                 =  6,
    Bars_Solved                   =  7,
    Bars_OneFace                  =  8,
    Bars_Bars_Same                =  9,
    Bars_Bars_Different           = 10,
    Bars_Bars_Exclusive           = 11,
    Bars_OneBar                   = 12,
    BarSlash_OneFace              = 13,
    BarSlash_Bars                 = 14,
    BarSlash_OneBar_Parallel      = 15,
    BarSlash_OneBar_Perpendicular = 16,
};

vector<string> StateNames =
{
    "Slashes / OneFace",
    "Slashes / Bars",
    "Solved / Solved",
    "Solved / OneFace (Same)",
    "Solved / OneFace (Different)",
    "Solved / Bars",
    "Solved / OneBar",
    "Bars / Solved",
    "Bars / OneFace",
    "Bars / Bars (Same)",
    "Bars / Bars (Different)",
    "Bars / Bars (Exclusive)",
    "Bars / OneBar",
    "BarSlash / OneFace",
    "BarSlash / Bars",
    "BarSlash / OneBar (Parallel)",
    "BarSlash / OneBar (Perpendicular)",
};

vector<vector<int>> calcDistance()
{
    const vector<vector<int>> C =
    {
        //0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16
        { 0, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //  0
        { 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, //  1
        { 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}, //  2
        { 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0}, //  3
        { 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0}, //  4
        { 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0}, //  5
        { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0}, //  6
        { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0}, //  7
        { 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0}, //  8
        { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0}, //  9
        { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}, // 10
        { 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1}, // 11
        { 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0}, // 12
        { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 0, 0}, // 13
        { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0}, // 14
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1}, // 15
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1}, // 16
    };

    int oo = 99;
    vector<vector<int>> D(34, vector<int>(34, oo));
    for (int i=0; i<17; i++)
    {
        D[i*2][i*2] = 0;
        D[i*2+1][i*2+1] = 0;

        for (int j=0; j<17; j++)
            if (C[i][j]>0)
            {
                D[i*2][j*2+1] = 1;
                D[i*2+1][j*2] = 1;
            }
    }

    for (int i=0; i<34; i++)
        for (int j=0; j<34; j++)
            for (int k=0; k<34; k++)
                D[j][k] = min(D[j][k], D[j][i]+D[i][k]);

    vector<vector<int>> R(17, vector<int>(2));
    for (int i=0; i<17; i++)
        for (int j=0; j<2; j++)
            R[i][j] = D[2*2][i*2+j];

    return R;
}

string CP2str(vector<int> CP);

// https://docs.google.com/document/d/1Onc2lu5iW_ODnT71oN5lgpVVjVqbvUMzaylzxgykx0I/edit
State calcState(vector<int> CP)
{
    //    0------1
    //   /|     /|
    //  3-+----2 |
    //  | |    | |
    //  | 4----+-5
    //  |/     |/
    //  7------6

    string UD, FB;
    for (int p=0; p<8; p++)
    {
        UD += CP[p]<4?"U":"D";
        FB += (p==0||p==2||p==5||p==7)==(CP[p]==0||CP[p]==2||CP[p]==5||CP[p]==7)?"F":"R";
    }

    // UD
    int tc = 0;
    for (int p: {0, 2, 5, 7})
        if (UD[p]=='U')
            tc++;

    StateUD stateUD;
    if (tc==0 || tc==4)
        stateUD = StateUD::Slashes;
    else if (tc==1 || tc==3)
        stateUD = StateUD::BarSlash;
    else
    {
        // Solved は、上下が全て一致しているか、全て一致していないかのいずれか。
        int c = 0;
        for (int i=0; i<4; i++)
            if (UD[i]==UD[i+4])
                c++;
        if (c==0 || c==4)
            stateUD = StateUD::Solved;
        else
            stateUD = StateUD::Bars;
    }

    // FB
    int bc = 0;
    for (char fb: FB)
        if (fb=='R')
            bc++;
    assert(bc%2==0);

    StateFB stateFB;
    if (bc==0 || bc==8)
        stateFB = StateFB::Solved;
    else if (bc==2 || bc==6)
        stateFB = StateFB::OneBar;
    else
    {
        // OneFace は、前後が全て一致しているか、全て一致していないかのいずれか。
        int c =
            (FB[0]==FB[3]?1:0)+
            (FB[1]==FB[2]?1:0)+
            (FB[4]==FB[7]?1:0)+
            (FB[5]==FB[6]?1:0);
        if (c==0 || c==4)
            stateFB = StateFB::OneFace;
        else
            stateFB = StateFB::Bars;
    }

    State state;
    if (stateUD==StateUD::Slashes && stateFB==StateFB::OneFace)
        state = State::Slashes_OneFace;
    else if (stateUD==StateUD::Slashes && stateFB==StateFB::Bars)
        state = State::Slashes_Bars;
    else if (stateUD==StateUD::Solved && stateFB==StateFB::Solved)
        state = State::Solved_Solved;
    else if (stateUD==StateUD::Solved && stateFB==StateFB::OneFace)
    {
        // UとRを向けているピースが0個か4個ならば、same。
        int c = 0;
        for (int i=0; i<8; i++)
            if (UD[i]=='U' && FB[i]=='R')
                c++;
        assert(c%2==0);

        if (c==0 || c==4)
            state = State::Solved_OneFace_Same;
        else
            state = State::Solved_OneFace_Different;
    }
    else if (stateUD==StateUD::Solved && stateFB==StateFB::Bars)
        state = State::Solved_Bars;
    else if (stateUD==StateUD::Solved && stateFB==StateFB::OneBar)
        state = State::Solved_OneBar;
    else if (stateUD==StateUD::Bars && stateFB==StateFB::Solved)
        state = State::Bars_Solved;
    else if (stateUD==StateUD::Bars && stateFB==StateFB::OneFace)
        state = State::Bars_OneFace;
    else if (stateUD==StateUD::Bars && stateFB==StateFB::Bars)
    {
        // UとRを向けているピースが0個か4個ならばこれらでUDとFBの両方のバーを作れる。
        // Solved_OneFace に遷移するときのQTでRを向けているピースが変化するので、differentになる。
        int c = 0;
        for (int i=0; i<8; i++)
            if (UD[i]=='U' && FB[i]=='R')
                c++;
        if (c==0 || c==4)
            state = State::Bars_Bars_Different;
        else
        {
            // 適当に回して、UDとFBの両方のバーができるかどうかを調べる。
            string UD2 = UD;
            string FB2 = FB;

            state = State::Bars_Bars_Exclusive;
            for (int i=0; i<100; i++)
            {
                bool ud = UD2[0]==UD2[1] && UD2[1]!=UD2[2] && UD2[2]==UD2[3] || UD2[4]==UD2[5] && UD2[5]!=UD2[6] && UD2[6]==UD2[7] ||
                          UD2[0]==UD2[3] && UD2[3]!=UD2[2] && UD2[2]==UD2[1] || UD2[4]==UD2[7] && UD2[7]!=UD2[6] && UD2[6]==UD2[5];
                bool fb = FB2[0]==FB2[1] && FB2[1]!=FB2[2] && FB2[2]==FB2[3] || FB2[4]==FB2[5] && FB2[5]!=FB2[6] && FB2[6]==FB2[7] ||
                          FB2[0]==FB2[3] && FB2[3]!=FB2[2] && FB2[2]==FB2[1] || FB2[4]==FB2[7] && FB2[7]!=FB2[6] && FB2[6]==FB2[5];
                if (ud&&fb)
                {
                    state = State::Bars_Bars_Same;
                    break;
                }

                switch (rand()%3)
                {
                case 0:
                    swap(UD2[1], UD2[6]);
                    swap(UD2[2], UD2[5]);
                    swap(FB2[1], FB2[6]);
                    swap(FB2[2], FB2[5]);
                    break;
                case 1:
                    swap(UD2[2], UD2[7]);
                    swap(UD2[3], UD2[6]);
                    swap(FB2[2], FB2[7]);
                    swap(FB2[3], FB2[6]);
                    break;
                case 2:
                    swap(UD2[0], UD2[2]);
                    swap(UD2[1], UD2[3]);
                    swap(FB2[0], FB2[2]);
                    swap(FB2[1], FB2[3]);
                    break;
                }
            }
        }
    }
    else if (stateUD==StateUD::Bars && stateFB==StateFB::OneBar)
        state = State::Bars_OneBar;
    else if (stateUD==StateUD::BarSlash && stateFB==StateFB::OneFace)
        state = State::BarSlash_OneFace;
    else if (stateUD==StateUD::BarSlash && stateFB==StateFB::Bars)
        state = State::BarSlash_Bars;
    else if (stateUD==StateUD::BarSlash && stateFB==StateFB::OneBar)
    {
        // UDもFBもバーができるまで適当に回す。
        string UD2 = UD;
        string FB2 = FB;

        while (true)
        {
            bool udx = UD2[0]==UD2[1] && UD2[1]!=UD2[2] && UD2[2]==UD2[3] || UD2[4]==UD2[5] && UD2[5]!=UD2[6] && UD2[6]==UD2[7];
            bool udy = UD2[0]==UD2[3] && UD2[3]!=UD2[2] && UD2[2]==UD2[1] || UD2[4]==UD2[7] && UD2[7]!=UD2[6] && UD2[6]==UD2[5];
            bool fbx = FB2[0]==FB2[1] && FB2[1]!=FB2[2] && FB2[2]==FB2[3] || FB2[4]==FB2[5] && FB2[5]!=FB2[6] && FB2[6]==FB2[7];
            bool fby = FB2[0]==FB2[3] && FB2[3]!=FB2[2] && FB2[2]==FB2[1] || FB2[4]==FB2[7] && FB2[7]!=FB2[6] && FB2[6]==FB2[5];
            if ((udx||udy)&&(fbx||fby))
            {
                if (udx && fbx || udy && fby)
                    state = State::BarSlash_OneBar_Parallel;
                else if (udx && fby || udy && fbx)
                    state = State::BarSlash_OneBar_Perpendicular;
                else
                    assert(false);
                break;
            }

            switch (rand()%3)
            {
            case 0:
                swap(UD2[1], UD2[6]);
                swap(UD2[2], UD2[5]);
                swap(FB2[1], FB2[6]);
                swap(FB2[2], FB2[5]);
                break;
            case 1:
                swap(UD2[2], UD2[7]);
                swap(UD2[3], UD2[6]);
                swap(FB2[2], FB2[7]);
                swap(FB2[3], FB2[6]);
                break;
            case 2:
                swap(UD2[0], UD2[2]);
                swap(UD2[1], UD2[3]);
                swap(FB2[0], FB2[2]);
                swap(FB2[1], FB2[3]);
                break;
            }
        }
    }
    else
        assert(false);

    return state;
}

string CP2str(vector<int> CP)
{
    const vector<string> C = {"ULB", "UBR", "URF", "UFL", "DBL", "DRB", "DFR", "DLF"};

    auto c = [&](int p, int o) -> char
    {
        return C[CP[p]][o];
    };

    string res = string() +
    ' '     + ' '     + ' '     + c(0, 0) + '.'     + c(1, 0) + '\n' +
    ' '     + ' '     + ' '     + '.'     + 'U'     + '.'     + '\n' +
    ' '     + ' '     + ' '     + c(3, 0) + '.'     + c(2, 0) + '\n' +
    c(0, 1) + '.'     + c(3, 2) + c(3, 1) + '.'     + c(2, 2) + c(2, 1) + '.'     + c(1, 2) + c(1, 1) + '.'     + c(0, 2) + '\n' +
    '.'     + 'L'     + '.'     + '.'     + 'F'     + '.'     + '.'     + 'R'     + '.'     + '.'     + 'B'     + '.'     + '\n' +
    c(4, 2) + '.'     + c(7, 1) + c(7, 2) + '.'     + c(6, 1) + c(6, 2) + '.'     + c(5, 1) + c(5, 2) + '.'     + c(4, 1) + '\n' +
    ' '     + ' '     + ' '     + c(7, 0) + '.'     + c(6, 0) + '\n' +
    ' '     + ' '     + ' '     + '.'     + 'D'     + '.'     + '\n' +
    ' '     + ' '     + ' '     + c(4, 0) + '.'     + c(5, 0) + '\n';

    string colored;
    for (char c: res)
        switch (c)
        {
        case 'U': colored += "\x1b[48;5;15mU \x1b[0m"; break;
        case 'D': colored += "\x1b[48;5;3mD \x1b[0m"; break;
        case 'L': colored += "\x1b[48;5;202mL \x1b[0m"; break;
        case 'R': colored += "\x1b[48;5;1mR \x1b[0m"; break;
        case 'F': colored += "\x1b[48;5;2mF \x1b[0m"; break;
        case 'B': colored += "\x1b[48;5;4mB \x1b[0m"; break;
        case '.': colored += "\x1b[48;5;240m  \x1b[0m"; break;
        case ' ': colored += "  "; break;
        case '\n': colored += "\n"; break;
        default:
            colored += "ERROR";
        }

    return colored;
}

#include <cstdlib>

int main()
{
    vector<vector<int>> dist = calcDistance();

    vector<int> CP;
    for (int i=0; i<8; i++)
        CP.push_back(i);

    vector<vector<int>> numState(17, vector<int>(2));

    do
    {
        State state = calcState(CP);

        int parity = 0;
        vector<bool> U(8);
        for (int i=0; i<8; i++)
            if (!U[i])
            {
                int c = 0;
                int t = i;
                while (!U[t])
                {
                    U[t] = true;
                    c++;
                    t = CP[t];
                }
                if (c%2==0)
                    parity ^= 1;
            }

        numState[int(state)][parity]++;
    }
    while(next_permutation(CP.begin(), CP.end()));

    vector<int> numQT(6);

    for (int i=0; i<17; i++)
        for (int p=0; p<2; p++)
        {
            cout<<StateNames[i]<<" "<<p<<" "<<dist[i][p]<<" "<<numState[i][p]<<endl;
            numQT[dist[i][p]] += numState[i][p];
        }

    for (int qt=0; qt<=5; qt++)
        cout<<qt<<" "<<numQT[qt]<<endl;
}
