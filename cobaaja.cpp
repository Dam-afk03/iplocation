#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// Mengecek bagian dari IP (0-255)
void periksaFormatIP(const string& bagian) {
    if (bagian.empty() || bagian.find_first_not_of("0123456789") != string::npos) {
        throw runtime_error("Bagian IP tidak valid.");
    }

    int angka = stoi(bagian);
    if (angka < 0 || angka > 255) {
        throw runtime_error("Nilai IP di luar jangkauan (0-255).");
    }
}

// Mengubah string IP (misal: "192.168.1.1") ke bentuk angka 32-bit
unsigned int ubahKeAngkaIP(const string& ip) {
    unsigned int hasil = 0;
    size_t posisi = 0, awal = 0;

    for (int i = 0; i < 4; ++i) {
        posisi = ip.find('.', awal);
        string segmen = (posisi == string::npos) ? ip.substr(awal) : ip.substr(awal, posisi - awal);

        periksaFormatIP(segmen);
        int oktet = stoi(segmen);
        hasil = (hasil << 8) | oktet;
        awal = posisi + 1;
    }

    return hasil;
}

// Mencari lokasi berdasarkan IP yang telah diubah menjadi angka
bool cariLokasiIP(const string& namafile, unsigned int ipAngka) {
    ifstream berkas(namafile);
    if (!berkas.is_open()) {
        cerr << "Tidak dapat membuka file: " << namafile << "\n";
        return false;
    }

    string baris;
    while (getline(berkas, baris)) {
        stringstream ss(baris);
        string elemen;
        string data[6];
        int hitung = 0;

        while (getline(ss, elemen, ',') && hitung < 6) {
            data[hitung++] = elemen;
        }

        if (hitung == 6) {
            string cidr = data[0];
            size_t posisiSlash = cidr.find('/');
            if (posisiSlash == string::npos) continue;

            string ipDasar = cidr.substr(0, posisiSlash);
            int prefix = stoi(cidr.substr(posisiSlash + 1));

            unsigned int ipAwal = ubahKeAngkaIP(ipDasar);
            unsigned int ipAkhir = ipAwal | ((1u << (32 - prefix)) - 1);

            if (ipAngka >= ipAwal && ipAngka <= ipAkhir) {
                cout << "Negara   : " << data[1] << "\n";
                cout << "Kota     : " << data[2] << "\n";
                cout << "Wilayah  : " << data[3] << "\n";
                cout << "Latitude : " << data[4] << "\n";
                cout << "Longitude: " << data[5] << "\n";
                return true;
            }
        }
    }

    return false;
}

int main() {
    string namaFile = "IPlocation.txt";
    string ipPengguna;
    string jawab;

    cout << "Masukkan alamat IP (atau ketik 'exit' untuk keluar): ";
    while (cin >> ipPengguna && ipPengguna != "exit") {
        try {
            unsigned int ipDalamAngka = ubahKeAngkaIP(ipPengguna);

            if (!cariLokasiIP(namaFile, ipDalamAngka)) {
                cout << "Lokasi tidak ditemukan.\n";
            }

            while (true) {
                cout << "Ingin cek IP lain? (y/t): ";
                cin >> jawab;

                if (jawab == "y" || jawab == "Y") {
                    break;
                } else if (jawab == "t" || jawab == "T") {
                    cout << "Program dihentikan.\n";
                    return 0;
                } else {
                    cout << "Masukan tidak dikenali. Harap ketik 'y' atau 't'.\n";
                }
            }

        } catch (const exception& e) {
            cerr << "Terjadi kesalahan: " << e.what() << "\n";
        }

        cout << "Masukkan alamat IP (atau 'exit' untuk keluar): ";
    }

    return 0;
}
