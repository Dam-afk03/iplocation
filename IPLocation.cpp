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

unsigned int ubahkeAngkaip(const string& ip) {
    unsigned int ip_num = 0;
    size_t pos = 0, prev = 0;

    for (int i = 0; i < 4; ++i) {
        pos = ip.find('.', prev);
        string part = (pos == string::npos) ? ip.substr(prev) : ip.substr(prev, pos - prev);

        validasiIp(part);

        int octet = stoi(part);
        ip_num = (ip_num << 8) | octet;
        prev = pos + 1;
    }

    return ip_num;
}

bool ipLocationSearch(const string& filename, unsigned int ip_num) {
    ifstream file(filename);
    
    if (!file.is_open()) {
        cerr << "Gagal membuka file: " << filename << "\n";
        return false;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string field;
        string fields[6];
        int field_count = 0;

        
        while (getline(ss, field, ',') && field_count < 6) {
            fields[field_count++] = field;
        }

        if (field_count >= 6) {
            string cidr = fields[0];
            size_t slash = cidr.find('/');
            if (slash == string::npos) continue; 

            string ip_part = cidr.substr(0, slash);
            int prefix = stoi(cidr.substr(slash + 1));

            unsigned int start_ip = ubahkeAngkaip(ip_part);
            unsigned int end_ip = start_ip | ((1u << (32 - prefix)) - 1);

            if (ip_num >= start_ip && ip_num <= end_ip) {
                cout << "Negara : " << fields[1] << "\n";
                cout << "Kota   : " << fields[2] << "\n";
                cout << "Wilayah : " << fields[3] << "\n";
                cout << "Latitude : " << fields[4] << "\n";
                cout << "longitude : " << fields[5] << "\n";
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
            unsigned int ip_num = ubahkeAngkaip(ip_input);
            
            if (!ipLocationSearch(filename, ip_num)) {
                cout << "Lokasi tidak ditemukan untuk IP tersebut.\n";
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