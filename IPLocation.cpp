#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

void validasiIp(const string& part){
    if (part.empty() || part.find_first_not_of("0123456789") != string::npos)
            throw runtime_error("Format IP tidak valid");

        int angka = stoi(part);
        if (angka < 0 || angka > 255)
            throw runtime_error("IP tidak valid");
}

unsigned int ubahKeAngkaIp(const string& ip) {
    unsigned int ipAngka = 0;
    size_t posisi = 0, awal = 0;

    for (int i = 0; i < 4; ++i) {
        posisi = ip.find('.', awal);
        string part = (posisi == string::npos) ? ip.substr(awal) : ip.substr(awal, posisi - awal);

        validasiIp(part);

        int octet = stoi(part);
        ipAngka = (ipAngka << 8) | octet;
        awal = posisi + 1;
    }

    return ipAngka;
}

bool carilokasiIP(const string& filename, unsigned int ipAngka) {
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Gagal membuka file: " << filename << "\n";
        return false;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string field;
        string data[6];
        int hitung = 0;

        
        while (getline(ss, field, ',') && hitung < 6) {
            data[hitung++] = field;
        }

        if (hitung >= 6) {
            string cidr = data[0];
            size_t slash = cidr.find('/');
            if (slash == string::npos) continue; 

            string ipDasar = cidr.substr(0, slash);
            int prefix = stoi(cidr.substr(slash + 1));

            unsigned int ipAwal = ubahKeAngkaIp(ipDasar);
            unsigned int ipAkhir = ipAwal | ((1u << (32 - prefix)) - 1);

            if (ipAngka >= ipAwal && ipAngka <= ipAkhir) {
                cout << "Negara : " << data[1] << "\n";
                cout << "Kota   : " << data[2] << "\n";
                cout << "Wilayah : " << data[3] << "\n";
                cout << "Latitude : " << data[4] << "\n";
                cout << "longitude : " << data[5] << "\n";
                return true;
            }
        }
    }

    return false;
}

int main() {
    string filename = "IPlocation.txt";
    string ip_input;
    string pilihan;
    
    cout << "Masukkan IP atau 'exit' untuk keluar dari program: ";
    while (cin >> ip_input && ip_input != "exit") {
        try {
            unsigned int ipAngka = ubahKeAngkaIp(ip_input);
            
            if (!carilokasiIP(filename, ipAngka)) {
                cout << "Lokasi tidak ditemukan.\n";
            }
            while (true){
            cout << "lanjut untuk cek IP lain y/t: ";
            cin >> pilihan;
                if (pilihan == "y" || pilihan == "Y"){
                    break;
                }else if(pilihan == "t" || pilihan == "T"){
                    cout << "Keluar dari program.\n";
                return 0;
                }else{
                    cout << "Pilihan tidak valid. silahkan pilih y/t .\n";
                }
        
            }

        } catch (exception& e) {
            cerr << "Error: " << e.what() << "\n";
        }

        cout << "Masukkan IP atau 'exit' untuk keluar dari program: " ;
    }

    return 0;
}