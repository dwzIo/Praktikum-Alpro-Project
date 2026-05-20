#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

const int MAX_RAK = 5, MAX_SLOT = 20;

struct buku {
    string namaBuku, namaPenerbit, kategori;
    int jumlahBest, jumlahStok;
    bool isTersedia;
};

struct pelanggan {
    string namaPelanggan, tanggal, namaBukuDibeli;
    int jumlahBeli;
};

buku rakBuku[MAX_RAK][MAX_SLOT];

// Helper untuk transisi menu agar hemat baris
void aksiSelesai() {
    cout << "-------------------------------------------\n";
    system("pause"); system("cls");
}

void saveDatabaseBuku() {
    ofstream file("database_buku.csv", ios::trunc);
    if (!file.is_open()) return;
    for (int i = 0; i < MAX_RAK; i++) {
        for (int j = 0; j < MAX_SLOT; j++) {
            if (rakBuku[i][j].isTersedia) {
                file << i << ";" << j << ";" << rakBuku[i][j].namaBuku << ";" << rakBuku[i][j].namaPenerbit << ";"
                     << rakBuku[i][j].kategori << ";" << rakBuku[i][j].jumlahBest << ";" << rakBuku[i][j].jumlahStok << ";" 
                     << rakBuku[i][j].isTersedia << "\n";
            }
        }
    }
    file.close();
}

void loadDatabaseBuku() {
    ifstream file("database_buku.csv");
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string sR, sS, nama, pen, kat, sB, sSt, sAv;
        getline(ss, sR, ';'); getline(ss, sS, ';'); getline(ss, nama, ';'); getline(ss, pen, ';');
        getline(ss, kat, ';'); getline(ss, sB, ';'); getline(ss, sSt, ';'); getline(ss, sAv, ';');
        
        int r = atoi(sR.c_str()), s = atoi(sS.c_str());
        if (r >= 0 && r < MAX_RAK && s >= 0 && s < MAX_SLOT) {
            rakBuku[r][s] = {nama, pen, kat, atoi(sB.c_str()), atoi(sSt.c_str()), sAv == "1"};
        }
    }
    file.close();
}

void saveTransaksi(pelanggan p) {
    ofstream file("database_transaksi.csv", ios::app);
    if (file.is_open()) {
        file << p.namaPelanggan << ";" << p.tanggal << ";" << p.namaBukuDibeli << ";" << p.jumlahBeli << "\n";
        file.close();
    }
}

void lihatRiwayatPembelian() {
    ifstream file("database_transaksi.csv");
    cout << "=== RIWAYAT PEMBELIAN BUKU ===\n";
    if (!file.is_open()) {
        cout << "[ Belum ada data transaksi ]\n";
        aksiSelesai(); return;
    }
    string line; bool ada = false;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line); string n, t, j, sJ;
        getline(ss, n, ';'); getline(ss, t, ';'); getline(ss, j, ';'); getline(ss, sJ, ';');
        cout << " -> Pelanggan: " << n << " | Tgl: " << t << " | Buku: " << j << " | Jml: " << sJ << " pcs\n";
        ada = true;
    }
    if (!ada) cout << "[ Belum ada data transaksi ]\n";
    file.close(); aksiSelesai();
}

void cariBukuJudul() {
    string keyword; cout << "=== CARI JUDUL BUKU ===\nMasukkan Judul: ";
    cin.ignore(); getline(cin, keyword);
    bool f = false;
    for (int i = 0; i < MAX_RAK; i++) {
        for (int j = 0; j < MAX_SLOT; j++) {
            buku* ptrBuku = &rakBuku[i][j];
            if (ptrBuku->isTersedia && ptrBuku->namaBuku.find(keyword) != string::npos) {
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
    int r, s, jmlBeli; pelanggan p;
    cout << "=== BELI BUKU ===\nMasukkan No Rak & Slot (misal: 1 2): "; cin >> r >> s;
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
        cout << "=== KATEGORI BUKU ===\n1. Fiksi\n2. Non-Fiksi\n3. Kembali\nPilih: "; cin >> pil;
        if (pil == 3) { system("cls"); break; }
        system("cls"); string katTarget = (pil == 1) ? "Fiksi" : "Non-Fiksi";
        cout << "=== " << katTarget << " ===\n"; bool ketemu = false;
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
    while (true) {
        cout << "=== MENU ADMIN ===\n1. Tambah Buku\n2. Tambah Stok\n3. Hapus Buku\n4. Riwayat Transaksi\n5. Exit\nPilih: "; cin >> pil;
        if (pil == 5) { system("cls"); break; }
        system("cls");
        if (pil == 4) { lihatRiwayatPembelian(); continue; }
        cout << "Masukkan No Rak & Slot: "; cin >> r >> s; r--; s--;
        if (r < 0 || r >= MAX_RAK || s < 0 || s >= MAX_SLOT) { cout << "Koordinat di luar batas!\n"; aksiSelesai(); continue; }
        
        if (pil == 1) {
            if (rakBuku[r][s].isTersedia) cout << "Slot sudah terisi!\n";
            else {
                cin.ignore(); cout << "Judul: "; getline(cin, rakBuku[r][s].namaBuku);
                cout << "Penerbit: "; getline(cin, rakBuku[r][s].namaPenerbit);
                cout << "Kategori: "; getline(cin, rakBuku[r][s].kategori);
                cout << "Stok: "; cin >> rakBuku[r][s].jumlahStok;
                rakBuku[r][s].jumlahBest = 0; rakBuku[r][s].isTersedia = true;
                saveDatabaseBuku(); cout << "\nBuku berhasil didaftarkan!\n";
            }
        } else if (pil == 2 && rakBuku[r][s].isTersedia) {
            cout << "Stok lama: " << rakBuku[r][s].jumlahStok << "\nTambahan Stok: "; cin >> tam;
            rakBuku[r][s].jumlahStok += tam; saveDatabaseBuku(); cout << "Stok diperbarui!\n";
        } else if (pil == 3 && rakBuku[r][s].isTersedia) {
            rakBuku[r][s].isTersedia = false; saveDatabaseBuku(); cout << "Buku dihapus.\n";
        } else cout << "Buku tidak ditemukan/pilihan salah!\n";
        aksiSelesai();
    }
}

void halamanUser() {
    int pil;
    while (true) {
        cout << "\n=== TOKO BUKU SINAR ABADI ===\n"
             << "1. Daftar Semua Buku\n"
             << "2. Cari via Kategori\n"
             << "3. Cari Judul\n"
             << "4. Best Seller\n"
             << "5. Beli Buku\n"
             << "6. Riwayat Pembelian\n"
             << "7. Exit\nPilih: "; cin >> pil;
        if (pil == 7) { system("cls"); break; }
        system("cls");
        if (pil == 1) daftarbuku();
        else if (pil == 2) kategoribuku();
        else if (pil == 3) cariBukuJudul();
        else if (pil == 4) sortBukuBestSeller();
        else if (pil == 5) beliBuku();
        else if (pil == 6) lihatRiwayatPembelian();
    }
}

bool prosesLogin(string type) {
    int maxAttempt = 3; string u, p;
    string usn[2] = {type == "A" ? "admindzakwan" : "dzakwan", type == "A" ? "adminaufa" : "aufa"};
    string pwd[2] = {type == "A" ? "admindzakwan123" : "dzakwan123", type == "A" ? "adminaufa123" : "aufa123"};
    cin.ignore();
    while (maxAttempt > 0) {
        cout << "=== LOGIN " << (type == "A" ? "ADMIN" : "MEMBER") << " ===\nUsername: "; getline(cin, u);
        cout << "Password: "; getline(cin, p);
        for (int i = 0; i < 2; i++) {
            if (u == usn[i] && p == pwd[i]) {
                cout << "Login Berhasil!\n"; aksiSelesai();
                if (type == "A") halamanAdmin(); else halamanUser();
                return true;
            }
        }
        maxAttempt--; cout << "Salah! Sisa percobaan: " << maxAttempt << "\n\n";
    }
    return false;
}

int main() {
    loadDatabaseBuku(); int pil;
    while (true) {
        cout << "=== MENU UTAMA ===\n"
             << "1. Pelanggan / Member\n"
             << "2. Petugas Admin\n"
             << "3. Keluar\n"
             << "Pilih: "; cin >> pil;
        system("cls");
        if (pil == 3) { cout << "Sampai jumpa!\n"; break; }
        system("cls");
        if (pil == 1) {
            char ans; cout << "Masuk akun membership (y/n)? "; cin >> ans;
            if (ans == 'y' || ans == 'Y') prosesLogin("U"); else { system("cls"); halamanUser(); }
        } else if (pil == 2) prosesLogin("A");
    }
    return 0;
}