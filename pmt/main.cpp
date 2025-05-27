#include <iostream>
#include <vector>
#include <random>
#include <cmath>
using namespace std;

vector<double> generisi_uniformni_niz(int N) {
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0);
    vector<double> niz(N);
    for(int i=0; i<N; i++) {
        niz[i]=dis(gen)-0.5;
    }
    return niz;
}

double neuniformni_kvantizator(double x) {
    if (x>=-1 && x<-0.382) return -0.588;
    else if (x>=-0.382 && x<0) return -0.176;
    else if (x>=0 && x<0.382) return 0.176;
    else if (x>=0.382 && x<=1) return 0.588;
    return 0;
}

vector<double> primeni_neuniformnu_kvantizaciju(const vector<double>& signal) {
    vector<double> kvantizovani_signal(signal.size());
    for(size_t i=0; i<signal.size(); ++i) {
        kvantizovani_signal[i]=neuniformni_kvantizator(signal[i]);
    }
    return kvantizovani_signal;
}

double izracunaj_srednju_snagu(const vector<double>& signal) {
    double suma_snage=0.0;
    for(size_t i=0; i<signal.size(); ++i) {
        suma_snage+=signal[i]*signal[i];
    }
    return suma_snage/signal.size();
}

vector<double> saberi_vektore(const vector<double>& vektor1, const vector<double>& vektor2) {
    vector<double> rezultat(vektor1.size());
    for(size_t i=0; i<vektor1.size(); i++) {
        rezultat[i]=vektor1[i]+vektor2[i];
    }
    return rezultat;
}

vector<double> proceni_verovatnocu(const vector<double>& signal, int Nint) {
    vector<int> brojac(Nint, 0);
    vector<double> verovatnoca(Nint, 0.0);
    int N=signal.size();
    double duzina_intervala=2.0/Nint;
    for(size_t i=0; i<signal.size(); i++) {
        double odbirak=signal[i];
        int index=(odbirak +1)/duzina_intervala;
        if (index >= Nint) index=Nint-1;
        brojac[index]++;
    }
    for(int i=0; i<Nint; i++) {
        verovatnoca[i]=static_cast<double>(brojac[i])/N;
    }
    return verovatnoca;
}

double uniformni_kvantizator(double x, int q) {
    double step = 2.0 / q;
    double kvantizovana_vrednost = floor((x + 1) / step) * step - 1 + step / 2;
    if (kvantizovana_vrednost < -1) kvantizovana_vrednost = -1 + step/2;
    if (kvantizovana_vrednost > 1) kvantizovana_vrednost = 1 - step/2;
    return kvantizovana_vrednost;
}


vector<double> primeni_uniformnu_kvantizaciju(const vector<double>& signal, int q) {
    vector<double> kvantizovani_signal(signal.size());
    for(size_t i=0; i<signal.size(); ++i) {
        kvantizovani_signal[i]=uniformni_kvantizator(signal[i], q);
    }
    return kvantizovani_signal;
}


int main() {
    int N=100000;
    int q=4;

    auto x1 =generisi_uniformni_niz(N);
    auto x2 =generisi_uniformni_niz(N);
    auto ulaznisignal =saberi_vektore(x1, x2);
    auto xq =primeni_neuniformnu_kvantizaciju(ulaznisignal);
    auto xqq =primeni_uniformnu_kvantizaciju(ulaznisignal, q);


    vector<double> greskan(N);
    for (int i=0; i<N; i++) {
        greskan[i]=ulaznisignal[i]-xq[i];
    }
    vector<double> greskau(N);
    for (int i=0; i<N; i++) {
        greskau[i]=ulaznisignal[i]-xqq[i];
    }

    double suma_kvadrata_gresakan = 0.0;
    for (size_t i = 0; i < greskan.size(); ++i) {
        suma_kvadrata_gresakan += greskan[i] * greskan[i];
    }
    double PNqn = suma_kvadrata_gresakan / N;

    double suma_kvadrata_gresakau = 0.0;
    for (size_t i = 0; i < greskau.size(); ++i) {
        suma_kvadrata_gresakau += greskau[i] * greskau[i];
    }
    double PNqu = suma_kvadrata_gresakau / N;

    double snaga_signalan=izracunaj_srednju_snagu(ulaznisignal);
    double snaga_signalau=4./12;
    double SNRqn=10*log10(snaga_signalan/PNqn);
    double SNRqu=10*log10(snaga_signalau/PNqu);

    cout<<"Ispis rezultata:"<<endl;
    cout<<"Srednja snaga odbiraka na ulazu u kvantizer(neravnomena kvantizacija): "<<snaga_signalan<<endl;
    cout<<"Srednja snaga odbiraka na ulazu u kvantizer(ravnomena kvantizacija): "<<snaga_signalau<<endl;
    cout << "Srednja kvadratna vrednost greske kvantizacije (PNq(neravnomerna kvantizacija)): " << PNqn << endl;
    cout << "Odnos signal-noise ratio (SNRq(neravnomerna kvantizacija)) u dB: " << SNRqn << endl;
    cout << "Srednja kvadratna vrednost greske kvantizacije (PNq(ravnomerna kvantizacija)): " << PNqu << endl;
    cout << "Odnos signal-noise ratio (SNRq(ravnomerna kvantizacija)) u dB: " << SNRqu << endl;

    int Nint=20;
    int i=1;
    vector<double> novivektor =proceni_verovatnocu(ulaznisignal, Nint);
    cout<<"Verovatnoce pojavljivanja odbiraka:"<<endl;
    for(const auto& vrednost : novivektor) {
        cout<<"Nivo "<<i<<":";
        cout<<vrednost*100<<"%"<<endl;
        i++;
    }
    return 0;
}

