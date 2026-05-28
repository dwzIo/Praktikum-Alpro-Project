#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

struct buku {
    string namaBuku;
    string namaPenerbit;
    string kategori;
    int jumlahBest;
    int jumlahStok;
    bool isTersedia;
};

struct pelanggan {
    string namaPelanggan;
    string tanggal;
    string namaBukuDibeli;
    int jumlahBeli;
};

const int MAX_RAK = 5, MAX_SLOT = 20;
const string File_Admin = ("DatabaseAdmin.txt");
const string File_User = ("DatabaseUser.txt");
buku rakBuku[MAX_RAK][MAX_SLOT];

string replaceChar(string text, char source, char target){
    string newtext = text;
    int panjang = text.length();
    for(int i = 0; i < panjang; i ++){
        if(text[i] == source)
            newtext[i] = target;   
    }
    return newtext;
}

string replaceSpacetoUnderscore(string text){
    return replaceChar(text, ' ', '_');
}

string replaceUnderscoretoSpace(string text){
    return replaceChar(text, '_', ' ');
}

void aksiSelesai() {
    cout << "-------------------------------------------\n";
    system("pause"); system("cls");
}

void loadDatabaseBuku() {
    ifstream file(File_Admin);
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string temp;
        buku buku_temp;
        replaceUnderscoretoSpace(buku_temp.namaBuku);
        replaceUnderscoretoSpace(buku_temp.namaPenerbit);

        getline(ss, buku_temp.namaBuku, ' ');
        getline(ss, buku_temp.namaPenerbit, ' ');
        getline(ss, buku_temp.kategori, ' ');
        getline(ss, temp, ' '); if (!temp.empty()) buku_temp.jumlahBest = stoi(temp);
        getline(ss, temp, ' '); if (!temp.empty()) buku_temp.jumlahStok = stoi(temp);
        getline(ss, temp, ' '); if (!temp.empty()) buku_temp.isTersedia = (temp == "true" ? true : false);

    }
    file.close();
}

void saveDatabaseBuku() {
    ofstream file(File_Admin, ios::trunc);
    if (!file.is_open()) return;
    for (int i = 0; i < MAX_RAK; i++) {
        for (int j = 0; j < MAX_SLOT; j++) {
            if (rakBuku[i][j].isTersedia) {
                replaceSpacetoUnderscore(rakBuku[i][j].namaBuku);
                replaceSpacetoUnderscore(rakBuku[i][j].namaPenerbit);

                file << i << " " << j << " " << rakBuku[i][j].namaBuku << " " << rakBuku[i][j].namaPenerbit << " "
                     << rakBuku[i][j].kategori << " " << rakBuku[i][j].jumlahBest << " " << rakBuku[i][j].jumlahStok << " " 
                     << rakBuku[i][j].isTersedia << "\n";
            }
        }
    }
    file.close();
}

void lihatRiwayatPembelian() {
    ifstream file(File_User);
    cout << "=== RIWAYAT PEMBELIAN BUKU ===\n";
    if (!file.is_open()) {
        cout << "[ Belum ada data transaksi ]\n";
        aksiSelesai(); return;
    }
    string line; bool ada = false;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string temp;
        pelanggan pelanggan_temp;
        replaceUnderscoretoSpace(pelanggan_temp.namaPelanggan);
        replaceUnderscoretoSpace(pelanggan_temp.namaBukuDibeli);

        getline(ss, pelanggan_temp.namaPelanggan, ';');
        getline(ss, pelanggan_temp.tanggal, ';');
        getline(ss, pelanggan_temp.namaBukuDibeli, ';');
        getline(ss, temp, ';'); if (!temp.empty()) pelanggan_temp.jumlahBeli = stoi(temp); 
        cout << " -> Pelanggan: " << pelanggan_temp.namaPelanggan << " | Tgl: " << pelanggan_temp.tanggal << " | Buku: " << pelanggan_temp.namaBukuDibeli << " | Jml: " << pelanggan_temp.jumlahBeli << " pcs\n";
        ada = true;
    }
    if (!ada) cout << "[ Belum ada data transaksi ]\n";
    file.close(); aksiSelesai();
}

void saveTransaksi(pelanggan p) {
    ofstream file(File_User, ios::app);
    if (!file.is_open()) return;

    replaceSpacetoUnderscore(p.namaPelanggan);
    replaceSpacetoUnderscore(p.namaBukuDibeli);
    file << p.namaPelanggan << " " << p.tanggal << " " << p.namaBukuDibeli << " " << p.jumlahBeli << "\n";
    file.close();
}

void cariBukuJudul() {
    string keyword;
    cout << "=== CARI JUDUL BUKU ===\n"
         << "Masukkan Judul: ";
            getline(cin, keyword);
            cin.ignore();
    bool f = false;
    for (int i = 0; i < MAX_RAK; i++) {
        for (int j = 0; j < MAX_SLOT; j++) {
            buku* ptrBuku = &rakBuku[i][j];
            if (ptrBuku->isTersedia && ptrBuku->namaBuku.find(keyword) > -1) {
                cout << " -> [Rak " << i+1 << ", Slot " << j+1 << "] " << ptrBuku->namaBuku 
                     << " | Penerbit: " << ptrBuku->namaPenerbit << " | Stok: " << ptrBuku->jumlahStok << " pcs\n";
                f = true;
            }
        }
    }
    if (!f) cout << "Buku \"" << keyword << "\" tidak ditemukan.\n";
    aksiSelesai();
}

void sortBukuBestSeller() {
    buku* daftarPointer[MAX_RAK * MAX_SLOT]; int jml = 0;
    for (int i = 0; i < MAX_RAK; i++)
        for (int j = 0; j < MAX_SLOT; j++)
            if (rakBuku[i][j].isTersedia) daftarPointer[jml++] = &rakBuku[i][j];

    if (jml == 0) { cout << "Belum ada data buku!\n"; aksiSelesai(); return; }

    for (int i = 0; i < jml - 1; i++)
        for (int j = 0; j < jml - i - 1; j++)
            if (daftarPointer[j]->jumlahBest < daftarPointer[j+1]->jumlahBest)
                swap(daftarPointer[j], daftarPointer[j+1]);

    cout << "=== DAFTAR BUKU BEST SELLER ===\n";
    for (int i = 0; i < jml; i++)
        cout << " " << i+1 << ". Judul: " << daftarPointer[i]->namaBuku << " | Terjual: " << daftarPointer[i]->jumlahBest << " pcs | Sisa: " << daftarPointer[i]->jumlahStok << "\n";
    aksiSelesai();
}

void daftarbuku() {
    cout << "=== DAFTAR BUKU ===\n"; bool ada = false;
    for (int i = 0; i < MAX_RAK; i++) {
        for (int j = 0; j < MAX_SLOT; j++) {
            if (rakBuku[i][j].isTersedia) {
                cout << " -> [Rak " << i+1 << ", Slot " << j+1 << "] " << rakBuku[i][j].namaBuku << " | Stok: " << rakBuku[i][j].jumlahStok << " pcs\n";
                ada = true;
            }
        }
    }
    if (!ada) cout << "[ Seluruh Rak Buku Masih Kosong ]\n";
    aksiSelesai();
}

void beliBuku() {
    int r, s, jmlBeli;
    pelanggan p;
    cout << "=== BELI BUKU ===\n"
         << "Masukkan No Rak & Slot (misal: 1 2): "; cin >> r >> s;
    r--; s--;
    if (r >= 0 && r < MAX_RAK && s >= 0 && s < MAX_SLOT && rakBuku[r][s].isTersedia) {
        cout << "Buku: " << rakBuku[r][s].namaBuku << " (Stok: " << rakBuku[r][s].jumlahStok << ")\nJumlah Beli: "; cin >> jmlBeli;
        if (jmlBeli <= 0 || jmlBeli > rakBuku[r][s].jumlahStok) cout << "Jumlah tidak valid atau stok kurang!\n";
        else {
            cin.ignore(); cout << "Nama Anda: "; getline(cin, p.namaPelanggan);
            cout << "Tanggal (DD-MM-YYYY): "; getline(cin, p.tanggal);
            rakBuku[r][s].jumlahStok -= jmlBeli; rakBuku[r][s].jumlahBest += jmlBeli;
            p.namaBukuDibeli = rakBuku[r][s].namaBuku; p.jumlahBeli = jmlBeli;
            saveDatabaseBuku(); saveTransaksi(p);
            cout << "\nPembelian Berhasil!\n";
        }
    } else cout << "Buku tidak ditemukan!\n";
    aksiSelesai();
}

void kategoribuku() {
    int pil;
    while (true) {
        cout << "=== KATEGORI BUKU ===\n"
             << "1. Fiksi\n"
             << "2. Non-Fiksi\n"
             << "3. Kembali\n"
             << "Pilih: "; cin >> pil;
        system("cls");
        string katTarget = ((pil == 1) ? "Fiksi" : "Non-Fiksi");
        cout << "=== " << katTarget << " ===\n";
        bool ketemu = false;
        for (int i = 0; i < MAX_RAK; i++)
            for (int j = 0; j < MAX_SLOT; j++)
                if (rakBuku[i][j].isTersedia && (rakBuku[i][j].kategori == katTarget || rakBuku[i][j].kategori == (pil == 1 ? "fiksi" : "non-fiksi"))) {
                    cout << " -> [Rak " << i+1 << "][Slot " << j+1 << "] " << rakBuku[i][j].namaBuku << "\n";
                    ketemu = true;
                }
        if (!ketemu) cout << "Kategori ini belum tersedia.\n";
        aksiSelesai();
    }
}

void halamanAdmin() {
    int pil, r, s, tam;
    do{
        cout << "=== MENU ADMIN ===\n"
             << "1. Tambah Buku\n"
             << "2. Tambah Stok\n"
             << "3. Hapus Buku\n"
             << "4. Riwayat Transaksi\n"
             << "5. Exit\nPilih: "; cin >> pil;
        system("cls");
        if (pil == 4) { lihatRiwayatPembelian(); continue; }
        cout << "Masukkan No Rak & Slot: "; cin >> r >> s; r--; s--;
        if (r < 0 || r >= MAX_RAK || s < 0 || s >= MAX_SLOT) { cout << "Koordinat di luar batas!\n"; aksiSelesai(); continue; }
        
        if (pil == 1) {
            if (rakBuku[r][s].isTersedia) cout << "Slot sudah terisi!\n";
            else {
                cout << "Judul: "; getline(cin, rakBuku[r][s].namaBuku);
                cout << "Penerbit: "; getline(cin, rakBuku[r][s].namaPenerbit);
                cout << "Kategori: "; getline(cin, rakBuku[r][s].kategori);
                cout << "Stok: "; cin >> rakBuku[r][s].jumlahStok;
                cin.ignore();
                rakBuku[r][s].jumlahBest = 0;
                rakBuku[r][s].isTersedia = true;
                saveDatabaseBuku(); cout << "\nBuku berhasil didaftarkan!\n";
            }
        } else if (pil == 2 && rakBuku[r][s].isTersedia) {
            cout << "Stok lama: " << rakBuku[r][s].jumlahStok << "\nTambahan Stok: "; cin >> tam;
            rakBuku[r][s].jumlahStok += tam; saveDatabaseBuku(); cout << "Stok diperbarui!\n";
        } else if (pil == 3 && rakBuku[r][s].isTersedia) {
            rakBuku[r][s].isTersedia = false; saveDatabaseBuku(); cout << "Buku dihapus.\n";
        } else cout << "Buku tidak ditemukan/pilihan salah!\n";
        aksiSelesai();
    }while(pil != 5);
}

void halamanUser() {
    int pil;
    do{
        cout << "\n=== TOKO BUKU SINAR ABADI ===\n"
             << "1. Daftar Semua Buku\n"
             << "2. Cari via Kategori\n"
             << "3. Cari Judul\n"
             << "4. Best Seller\n"
             << "5. Beli Buku\n"
             << "6. Riwayat Pembelian\n"
             << "7. Exit\n"
             << "Pilih: "; cin >> pil;
        system("cls");
        if (pil == 1) daftarbuku();
        else if (pil == 2) kategoribuku();
        else if (pil == 3) cariBukuJudul();
        else if (pil == 4) sortBukuBestSeller();
        else if (pil == 5) beliBuku();
        else if (pil == 6) lihatRiwayatPembelian();
    }while(pil != 7);
}

bool prosesLogin(string type) {
    int maxAttempt = 3; string u, p;
    string usn[2] = {type == "A" ? "admindzakwan" : "dzakwan", type == "A" ? "adminaufa" : "aufa"};
    string pwd[2] = {type == "A" ? "admindzakwan123" : "dzakwan123", type == "A" ? "adminaufa123" : "aufa123"};
    cin.ignore();
    while (maxAttempt > 0) {
        cout << "=== LOGIN " << (type == "A" ? "ADMIN" : "MEMBER") << " ===\n"
             << "Username: "; getline(cin, u);
        cout << "Password: "; getline(cin, p);
        for (int i = 0; i < 2; i++) {
            if (u == usn[i] && p == pwd[i]) {
                cout << "Login Berhasil!\n"; aksiSelesai();
                if (type == "A") halamanAdmin();
                else halamanUser();
                return true;
            }
        }
        maxAttempt--;
        cout << "Salah! Sisa percobaan: " << maxAttempt << "\n\n";
    }
    return false;
}

int main() {
    loadDatabaseBuku(); int pil;
    do{
        cout << "=== MENU UTAMA ===\n"
             << "1. Pelanggan / Member\n"
             << "2. Petugas Admin\n"
             << "3. Keluar\n"
             << "Pilih: "; cin >> pil;
        system("cls");
        if (pil == 3) { cout << "Sampai jumpa!\n"; break; }
        system("cls");
        if (pil == 1) {
            char ans;
            cout << "Masuk akun membership (y/n)? "; cin >> ans;
            if (ans == 'y' || ans == 'Y') prosesLogin("U");
            else { system("cls"); halamanUser(); }
        } else if (pil == 2) prosesLogin("A");
    }while(pil != 3);
    return 0;
}