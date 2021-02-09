#include <iostream>
#include<fstream>
#include<cstring>
#include<list>
using namespace std;

/// Transformation of Nondeterministic Finite Automaton (NFA-lambda) into Deterministic Finite Automaton(DFA).

struct tranzitie{
    int nr_s1;                   /// nr_s1 = number of states from which the letter starts
    int *s1;                     /// *s1 = states from which the letter starts
    char litera;                 /// litera = the letter of the transition
    int nr_s2;                   /// nr_s2= number of states that receive the letter
    int *s2;                     /// *s2 = states that receive the letter
};

struct automat{
    int q;                        /// q = the total number of states
    int *init;                    /// *init = initial state, under vector type
    int nr_sf;                    /// nr_sf = number of final states
    int *sf;                      /// *sf = vector final states
    int l;                        /// l = number of letters in the alphabet
    char *L;                      /// *L = input alphabet
    int t;                        /// t = number of transitions
    tranzitie *T;                 /// *T = transition vector
};


struct ElementTabel {
    int *vect;
};

/// function to find out the lambda closing of the current state
/// (ie those states that reach the current state by reading lambda at any power, including 0)
void  InchidereLambda(int stare_curenta, int q,int t,tranzitie *T, int *inchiderelambda)
{
    int ok=1;
    inchiderelambda[stare_curenta]=1;  /// reaching it with lambda at power 0
    while(ok)                        /// as long as it changes inchiderelambda
    { ok=0;
      for (int i = 0; i < q; i++)
      {
        if(inchiderelambda[i]==1)
            for(int j=0;j<t;j++)
                if(T[j].s1[0]==i && T[j].litera=='&') /// &=lambda
                       for(int k=0;k<T[j].nr_s2;k++)
                            if(inchiderelambda[T[j].s2[k]]!=1)
                             {
                                 inchiderelambda[T[j].s2[k]]=1;
                                 ok=1;
                             }
    }
    }

}
/// function that calculates the closing for a letter
void InchidereLitera(int stare_curenta,int q,int t, tranzitie *T, int *inchiderelitera, char c)
{
    int *inchiderelambda=new int[q];
    for (int i = 0; i < q; i++)
        inchiderelitera[i]=0;
    InchidereLambda(stare_curenta,q,t,T,inchiderelambda);
    for(int i = 0; i < q; i++)
       {
        if(inchiderelambda[i]==1)
            for(int j=0;j<t;j++)
                if(T[j].s1[0]==i && T[j].litera==c)
                    for(int k=0;k<T[j].nr_s2;k++)
                      inchiderelitera[T[j].s2[k]]=1;
        }
}
/// function that will calculate lambda* letter lambda*
int * InchidereLambdaLiteraLambda(int stare_curenta,int q,int t, tranzitie *T, char c) {

    int *inchidere=new int[q];
    int *inchiderelitera=new int[q];
    int *inchiderelambda=new int[q];
    *inchidere={0};
    for (int i = 0; i < q; i++)
        inchiderelambda[i]=0;
    InchidereLambda(stare_curenta,q,t,T,inchiderelambda);

    for(int i = 0; i < q; i++) {
        if(inchiderelambda[i] == 1) {
            InchidereLitera(stare_curenta,q,t,T,inchiderelitera,c);
        }
    }

    for(int i = 0; i < q; i++) {
        if(inchiderelitera[i] == 1){
            InchidereLambda(i,q,t,T,inchidere);
        }
    }
    return inchidere;


}
/// function for the reunion of 2 vectors
void reuniuneMultimi(int a[], int b[], int rezultat[], int marime) {

    for (int i = 0; i < marime; i++)
        if (a[i] == 1 || b[i] == 1)
            rezultat[i] = 1;

}
/// check if 2 sets are equal
int verifegal(int *a, int *b, int marime) {
    for (int i = 0; i < marime; i++) {
        if(a[i] != b[i])
            return -1;
    }
    return 1;
}

/// transformation of NFA lambda into DFA
void AfnLambdaInAfd(automat AFNl, automat &AFD)
{   /// creating the table for the NFA lambda
    ElementTabel tabelAFNl[AFNl.q][AFNl.l];
    for (int i = 0; i < AFNl.q; i++) {
        for (int j = 0; j < AFNl.l; j++) {
            tabelAFNl[i][j].vect = InchidereLambdaLiteraLambda(i, AFNl.q, AFNl.t, AFNl.T, AFNl.L[j]);
        }
    }
    /* for printing NFA lambda table:
    for(int i = 0; i < AFNl.q; i++) {
        cout<<i<<" ";
        for(int j = 0; j < AFNl.l; j++) {
            for(int x = 0; x < AFNl.q; x++) {
                if(tabelAFNl[i][j].vect[x] == 1) {
                    cout<<x;
                }
            }
            cout<<"|";
        }
        cout<<endl;
    }
    */

    /// creating the table for the DFA
    ElementTabel tabelAFD[AFNl.q][AFNl.l + 1];       /// DFA table represents an array (table)
    ElementTabel elementInitial;                     /// the initial element in the table is the initial state of the DFA
    list<ElementTabel> elementeNoi;
    int linietabel=0;
    AFD.sf = new int[AFNl.q];
    AFD.init=new int[AFNl.q];
    elementInitial.vect = new int[AFNl.q];
    InchidereLambda(AFNl.init[0], AFNl.q, AFNl.t, AFNl.T, elementInitial.vect);
    /// the initial state of the DFA is the lambda closure of the initial state of the NFA lambda
    AFD.init=elementInitial.vect;
    elementeNoi.push_back(elementInitial);

    /// as long as new items can be added to the table
    while(!elementeNoi.empty())
    {
        ElementTabel elementNou = elementeNoi.front();
        tabelAFD[linietabel][0] = elementNou;
        elementeNoi.pop_front();
        for(int i = 0; i < AFNl.l; i++)
        {
            ElementTabel elementLitera;
            elementLitera.vect = new int[AFNl.q];
            for(int j = 0; j < AFNl.q; j++) {
                if(elementNou.vect[j] == 1){
                    reuniuneMultimi(elementLitera.vect, tabelAFNl[j][i].vect, elementLitera.vect, AFNl.q);
                }
            }

        int aparitie=0;

        for(int j = 0; j <=linietabel; j++)
            if(verifegal(tabelAFD[j][0].vect, elementLitera.vect, AFNl.q) == 1)
                aparitie = 1;

        for(ElementTabel x : elementeNoi)
            if(verifegal(x.vect, elementLitera.vect, AFNl.q) == 1)
                aparitie = 1;

        if(aparitie == 0)
            elementeNoi.push_back(elementLitera);

        tabelAFD[linietabel][i+1] = elementLitera;
        }

        linietabel++;

    }

    /* for printing the DFA table:
    for(int i = 0; i < linietabel; i++) {
        for(int j = 0; j < AFNl.l + 1; j++) {
            for(int x = 0; x < AFNl.q; x++) {
                if(tabelAFD[i][j].vect[x] == 1) {
                    cout<<x;
                }
            }
            cout<<"|";
        }
        cout<<endl;
    }
    */

    cout<<"DFA machine states: "<<endl;
    for(int i = 0; i < linietabel; i++)
    {
        for(int x = 0; x < AFNl.q; x++)
            if(tabelAFD[i][0].vect[x] == 1)
                cout<<x;
        cout<<endl;
    }

    AFD.l = AFNl.l;             /// DFA and NFA lambda have the same alphabet
    AFD.L = AFNl.L;
    AFD.t=linietabel*AFD.l;
    AFD.q = linietabel;
    AFD.nr_sf = 0;

    /// the final states of the DFA are the states that comprise at least one final state of the NFA lambda
    int StariFinale[AFNl.q] = {0};
    for(int i = 0; i < linietabel; i++)
        for(int j  = 0; j < AFNl.q; j++)
        {
            if(tabelAFD[i][0].vect[j] == 1)
                for(int k = 0; k < AFNl.nr_sf; k++)
                    if(AFNl.sf[k] == j)
                        StariFinale[i] = 1;
        }

    for(int i = 0; i < AFD.q; i++)
        if(StariFinale[i] == 1)
            AFD.nr_sf++;              /// calculating the number of final states

    int index = 0;
    for(int i = 0; i < AFD.q; i++)
        if(StariFinale[i] == 1)
            AFD.sf[index++] = i;

     /// we set the DFA transitions
     int tranz=0;
     AFD.T=new tranzitie[AFD.t];
     for(int i = 0; i < AFD.q; i++)
        for(int j = 0; j < AFD.l; j++)
            {   AFD.T[tranz].nr_s1=AFNl.q;
                AFD.T[tranz].s1=new int[AFD.T[tranz].nr_s1];
                AFD.T[tranz].s1=tabelAFD[i][0].vect;
                AFD.T[tranz].nr_s2=AFNl.q;
                AFD.T[tranz].s2=new int[AFD.T[tranz].nr_s2];
                AFD.T[tranz].s2=tabelAFD[i][j+1].vect;
                AFD.T[tranz].litera=AFNl.L[j];
                tranz++;
            }
}
int main()
{   ifstream f("date.txt");
    int i,j;
    automat AFD;
    automat AFNl;
    f>>AFNl.q;

    AFNl.init=new int[1];
    f>>AFNl.init[0];

    f>>AFNl.nr_sf;

    AFNl.sf=new int[AFNl.nr_sf];
    for(i=0;i<AFNl.nr_sf;i++)
        f>>AFNl.sf[i];
    f>>AFNl.l;
    AFNl.L=new char[AFNl.l];
    for(i=0;i<AFNl.l;i++)
        f>>AFNl.L[i];
    f>>AFNl.t;
    AFNl.T=new tranzitie[AFNl.t];
    for(i=0;i<AFNl.t;i++)
        {
        f>>AFNl.T[i].nr_s1;
        AFNl.T[i].s1=new int[AFNl.T[i].nr_s1];
        f>>AFNl.T[i].s1[0];
        f>>AFNl.T[i].litera>>AFNl.T[i].nr_s2;
        AFNl.T[i].s2=new int[AFNl.T[i].nr_s2];
        for(j=0;j<AFNl.T[i].nr_s2;j++)
            f>>AFNl.T[i].s2[j];
    }

    AfnLambdaInAfd(AFNl,AFD);

    cout<<"Number of states: "<<AFD.q<<endl;
    cout<<"Initial condition: ";
    for(int i=0;i<AFNl.q;i++)
        if(AFD.init[i]==1)
            cout<<i;
    cout<<endl;
    cout<<"Number of final states: "<<AFD.nr_sf<<endl;
    cout<<"Number of final states: ";
    for(int i=0;i<AFD.nr_sf;i++)
            cout<<AFD.sf[i]<<" ";
         cout<<endl;
    AFD.l=AFNl.l;
    strcpy(AFD.L,AFNl.L);
    cout<<"No. of letters in the alphabet: "<<AFD.l<<endl;
    cout<<"Letters in the alphabet: ";
    for(int i=0;i<AFD.l;i++)
        cout<<AFD.L[i]<<" ";
    cout<<endl;
    cout<<"Total number of transitions: "<<AFD.t<<endl;
    cout<<"The transitions are: ";
    cout<<endl;
    for(int i=0;i<AFD.t;i++)
      {
        cout<<i<<")    ";
        for(int j=0;j<AFD.T[i].nr_s1;j++)
            if(AFD.T[i].s1[j]==1)
              cout<<j;
        cout<<"--->";
        for(int j=0;j<AFD.T[i].nr_s2;j++)
            if(AFD.T[i].s2[j]==1)
                cout<<j;
        cout<<" with the letter ";
            cout<<AFD.T[i].litera<<endl;

      }
    return 0;
}
