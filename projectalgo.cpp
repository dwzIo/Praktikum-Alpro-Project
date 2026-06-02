#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
using namespace std;

struct buku {
    int rak;
    int slot;
    string namaBuku;
    string namaPenulis;
    int tahunTerbit;
    string kategori;
    int harga;
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

struct member {
    string username;
    string password;
    bool isTersedia;
};

struct admin {
    string username;
    string password;
};

const int MAX_RAK = 5, MAX_SLOT = 20;
const int MAX_MEMBER = 100;
const int MAX_ADMIN = 100;
const string File_Buku = ("DatabaseBuku.txt");
const string File_Riwayat = ("DatabaseRiwayat.txt");
const string File_Member = ("DatabaseMember.txt");
const string File_Admin = ("DatabaseAdmin.txt");
buku rakBuku[MAX_RAK][MAX_SLOT];
member membership[MAX_MEMBER];
admin staff[MAX_ADMIN];
int jumlah_member = 0;
int jumlah_admin = 0;

string replaceChar(string text, char source, char target){
    string newtext = text;
    for(int i = 0; i < text.length(); i ++){
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
    ifstream file(File_Buku);
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string temp;
        buku buku_temp;
        
        getline(ss, temp, ' '); if (!temp.empty()) buku_temp.rak = stoi(temp);
        getline(ss, temp, ' '); if (!temp.empty()) buku_temp.slot = stoi(temp);
        getline(ss, buku_temp.namaBuku, ' ');
        getline(ss, buku_temp.namaPenulis, ' ');
        getline(ss, temp, ' '); if (!temp.empty()) buku_temp.tahunTerbit = stoi(temp);
        getline(ss, buku_temp.kategori, ' ');
        getline(ss, temp, ' '); if (!temp.empty()) buku_temp.harga = stoi(temp);
        getline(ss, temp, ' '); if (!temp.empty()) buku_temp.jumlahBest = stoi(temp);
        getline(ss, temp, ' '); if (!temp.empty()) buku_temp.jumlahStok = stoi(temp);
        getline(ss, temp, ' '); if (!temp.empty()) buku_temp.isTersedia = (temp == "1" ? true : false);

        buku_temp.namaBuku = replaceUnderscoretoSpace(buku_temp.namaBuku);
        buku_temp.namaPenulis = replaceUnderscoretoSpace(buku_temp.namaPenulis);
        buku_temp.kategori = replaceUnderscoretoSpace(buku_temp.kategori);

        rakBuku[buku_temp.rak][buku_temp.slot] = buku_temp;
    }
    file.close();
}

void saveDatabaseBuku() {
    ofstream file(File_Buku, ios::trunc);
    if (!file.is_open()) return;
    for (int i = 0; i < MAX_RAK; i++) {
        for (int j = 0; j < MAX_SLOT; j++) {
            if (rakBuku[i][j].isTersedia) {
                rakBuku[i][j].namaBuku = replaceSpacetoUnderscore(rakBuku[i][j].namaBuku);
                rakBuku[i][j].namaPenulis = replaceSpacetoUnderscore(rakBuku[i][j].namaPenulis);
                rakBuku[i][j].kategori = replaceSpacetoUnderscore(rakBuku[i][j].kategori);

                file << i << " " << j << " " << rakBuku[i][j].namaBuku << " " << rakBuku[i][j].namaPenulis << " " << rakBuku[i][j].tahunTerbit << " "
                     << rakBuku[i][j].kategori << " " << rakBuku[i][j].harga << " "<< rakBuku[i][j].jumlahBest << " " << rakBuku[i][j].jumlahStok << " " 
                     << rakBuku[i][j].isTersedia << "\n";
            }
        }
    }
    file.close();
}

void lihatRiwayatPembelian() {
    ifstream file(File_Riwayat);
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

        getline(ss, pelanggan_temp.namaPelanggan, ' ');
        getline(ss, pelanggan_temp.tanggal, ' ');
        getline(ss, pelanggan_temp.namaBukuDibeli, ' ');
        getline(ss, temp, ' '); if (!temp.empty()) pelanggan_temp.jumlahBeli = stoi(temp);
        
        pelanggan_temp.namaPelanggan = replaceUnderscoretoSpace(pelanggan_temp.namaPelanggan);
        pelanggan_temp.namaBukuDibeli = replaceUnderscoretoSpace(pelanggan_temp.namaBukuDibeli);
        cout << "Pelanggan: " << pelanggan_temp.namaPelanggan << "\n"
             << "Tgl: " << pelanggan_temp.tanggal << "\n"
             << "Buku: " << pelanggan_temp.namaBukuDibeli << "\n"
             << "Jml: " << pelanggan_temp.jumlahBeli << " pcs\n";
        ada = true;
    }
    if (!ada) cout << "[ Belum ada data transaksi ]\n";
    file.close();
}

void saveTransaksi(pelanggan p) {
    ofstream file(File_Riwayat, ios::app);
    if (!file.is_open()) return;

    p.namaPelanggan = replaceSpacetoUnderscore(p.namaPelanggan);
    p.namaBukuDibeli = replaceSpacetoUnderscore(p.namaBukuDibeli);
    file << p.namaPelanggan << " " << p.tanggal << " " << p.namaBukuDibeli << " " << p.jumlahBeli << "\n";
    file.close();
}

void loadDatabaseMember() {
    ifstream file(File_Member);
    if (!file.is_open()) return;
    string line;
    jumlah_member = 0;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string temp;
        member member_temp;
        
        getline(ss, member_temp.username, ' ');
        getline(ss, member_temp.password, ' ');
        getline(ss, temp, ' '); if (!temp.empty()) member_temp.isTersedia = (temp == "1" ? true : false);

        member_temp.username = replaceUnderscoretoSpace(member_temp.username);
        member_temp.password = replaceUnderscoretoSpace(member_temp.password);
        membership[jumlah_member] = member_temp;
        jumlah_member++;
    }
    file.close();
}

void saveDatabaseMember(member m) {
    ofstream file(File_Member, ios::trunc);
    if (!file.is_open()) return;
    m.username = replaceSpacetoUnderscore(m.username);
    m.password = replaceSpacetoUnderscore(m.password);

    file << m.username << " " << m.password
         << " " << m.isTersedia << "\n";
    file.close();
    cout << "berhasil kamu langsung masuk sebagai membership\n";
}

void loadDatabaseAdmin() {
    ifstream file(File_Admin);
    if (!file.is_open()) return;
    string line;
    jumlah_admin = 0;
    while (getline(file, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string temp;
        admin admin_temp;
        
        getline(ss, admin_temp.username, ' ');
        getline(ss, admin_temp.password, ' ');

        admin_temp.username = replaceUnderscoretoSpace(admin_temp.username);
        admin_temp.password = replaceUnderscoretoSpace(admin_temp.password);
        staff[jumlah_admin] = admin_temp;
        jumlah_admin++;
    }
    file.close();
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
        cout << "Judul: " << daftarPointer[i]->namaBuku << "\n"
             << "Penulis: " << daftarPointer[i]->namaPenulis << "\n"
             << "Tahun Terbit: " << daftarPointer[i]->tahunTerbit << "\n"
             << "Kategori: " << daftarPointer[i]->kategori << "\n"
             << "Harga: " << daftarPointer[i]->harga << "\n"
             << "Terjual: " << daftarPointer[i]->jumlahBest << " pcs\n"
             << "Sisa: " << daftarPointer[i]->jumlahStok << "\n";
}

void daftarbuku() {
    cout << "=== DAFTAR BUKU ===\n"; bool ada = false;
    for (int i = 0; i < MAX_RAK; i++) {
        for (int j = 0; j < MAX_SLOT; j++) {
            if (rakBuku[i][j].isTersedia) {
                cout << " -> [Rak " << i+1 << ", Slot " << j+1 << "] " << "\n"
                     << "Nama Buku: " << rakBuku[i][j].namaBuku << "\n"
                     << "Penulis: " << rakBuku[i][j].namaPenulis << "\n"
                     << "Tahun Terbit: " << rakBuku[i][j].tahunTerbit << "\n"
                     << "Kategori: " << rakBuku[i][j].kategori << "\n"
                     << "Harga: " << rakBuku[i][j].harga << "\n"
                     << "Stok: " << rakBuku[i][j].jumlahStok << " pcs\n";
                ada = true;
            }
        }
    }
    if (!ada) cout << "[ Seluruh Rak Buku Masih Kosong ]\n";
}

void cariBukuJudul() {
    string keyword;
    cin.ignore(); 
    cout << "=== CARI JUDUL BUKU ===\n"
         << "Masukkan Judul: ";
    getline(cin, keyword);
    bool f = false;
    for (int i = 0; i < MAX_RAK; i++) {
        for (int j = 0; j < MAX_SLOT; j++) {
            buku* ptrBuku = &rakBuku[i][j];
            if ((ptrBuku->isTersedia) && (ptrBuku->namaBuku == keyword)) { //(int)ptrBuku->namaBuku.find(keyword) > -1
                cout << " -> [Rak " << i+1 << ", Slot " << j+1 << "]\n"
                     << "Nama Buku: " << ptrBuku->namaBuku << "\n"
                     << "Penulis: " << ptrBuku->namaPenulis << "\n"
                     << "Tahun Terbit: " << ptrBuku->tahunTerbit << "\n"
                     << "Kategori: " << ptrBuku->kategori << "\n"
                     << "Harga: " << ptrBuku->harga << "\n"
                     << "Stok: " << ptrBuku->jumlahStok << " pcs\n";
                f = true;
            }
        }
    }
    if (!f) cout << "Buku \"" << keyword << "\" tidak ditemukan.\n";
}

void beliBuku(float diskon) {
    int r, s, jmlBeli;
    pelanggan p;
    cout << "=== BELI BUKU ===\n"
         << "Masukkan No Rak & Slot (misal: 1 2): "; cin >> r >> s;
    r--; s--;
    if (r >= 0 && r < MAX_RAK && s >= 0 && s < MAX_SLOT && rakBuku[r][s].isTersedia) {
        cout << "Buku: " << rakBuku[r][s].namaBuku << "\n" << "Harga: " << rakBuku[r][s].harga * diskon << " (karena kamu adalah member diskon 5%)" << "\n" << "Stok: " << rakBuku[r][s].jumlahStok << "\nJumlah Beli: "; cin >> jmlBeli;
        if (jmlBeli <= 0 || jmlBeli > rakBuku[r][s].jumlahStok) cout << "Jumlah tidak valid atau stok kurang!\n";
        else {
            cin.ignore(); 
            cout << "Nama Anda: "; getline(cin, p.namaPelanggan);
            cout << "Tanggal (DD-MM-YYYY): "; getline(cin, p.tanggal);

            rakBuku[r][s].jumlahStok -= jmlBeli; rakBuku[r][s].jumlahBest += jmlBeli;
            p.namaBukuDibeli = rakBuku[r][s].namaBuku; p.jumlahBeli = jmlBeli;
            saveDatabaseBuku(); saveTransaksi(p);
            cout << "\nPembelian Berhasil!\n";
        }
    } else cout << "Buku tidak ditemukan!\n";
}

void kategoribuku() {
    int pil;
    do {
        cout << "=== KATEGORI BUKU ===\n"
             << "1. Fiksi\n"
             << "2. Non-Fiksi\n"
             << "3. Kembali\n"
             << "Pilih: "; cin >> pil;
        system("cls");
        if (pil == 3) {
            continue;
        }
        
        string katTarget = (pil == 1) ? "Fiksi" : "Non-Fiksi";
        string katTargetLower = (pil == 1) ? "fiksi" : "non-fiksi";
        cout << "=== " << katTarget << " ===\n";
        bool ketemu = false;
        for (int i = 0; i < MAX_RAK; i++)
            for (int j = 0; j < MAX_SLOT; j++)
                if (rakBuku[i][j].isTersedia && (rakBuku[i][j].kategori == katTarget || rakBuku[i][j].kategori == katTargetLower)) {
                    cout << " -> [Rak " << i+1 << "][Slot " << j+1 << "] " << "\n"
                         << "Nama Buku    : " << rakBuku[i][j].namaBuku << "\n" 
                         << "Harga (Diskon): " << rakBuku[i][j].harga << "\n"
                         << "Stok Tersedia: " << rakBuku[i][j].jumlahStok << " pcs\n---------------------\n";
                    ketemu = true;
                }
        if (!ketemu) cout << "Kategori ini belum tersedia.\n";
        aksiSelesai();
    } while (pil != 3);
}

void tambahbuku(int rak, int slot) {
    if (rakBuku[rak][slot].isTersedia) {
        cout << "Slot sudah terisi!\n";
        return;
    }
    else {
        cout << "Judul: "; getline(cin, rakBuku[rak][slot].namaBuku);
        cout << "Penulis: "; getline(cin, rakBuku[rak][slot].namaPenulis);
        cout << "Tahun Terbit: "; cin >> rakBuku[rak][slot].tahunTerbit;
        cin.ignore(); 
        cout << "Kategori: "; getline(cin, rakBuku[rak][slot].kategori);
        cout << "Harga: "; cin >> rakBuku[rak][slot].harga;
        cout << "Stok: "; cin >> rakBuku[rak][slot].jumlahStok;
        
        rakBuku[rak][slot].jumlahBest = 0;
        rakBuku[rak][slot].isTersedia = true;
        saveDatabaseBuku();
        cout << "\nBuku berhasil didaftarkan!\n";
    }
}

void tambahstok(int rak, int slot) {
    int tam;
    cout << "Stok lama: " << rakBuku[rak][slot].jumlahStok << "\n"
         << "Tambahan Stok: "; cin >> tam;

    rakBuku[rak][slot].jumlahStok += tam;
    saveDatabaseBuku();
    cout << "Stok diperbarui!\n";
}

void halamanAdmin() {
    int pil, r, s;
    do{
        cout << "=== MENU ADMIN ===\n"
             << "1. Daftar Semua Buku\n"
             << "2. Tambah Buku\n"
             << "3. Tambah Stok\n"
             << "4. Hapus Buku\n"
             << "5. Riwayat Transaksi\n"
             << "6. Exit\n"
             << "Pilih: "; cin >> pil;
        system("cls");
        if (pil == 1) {
            daftarbuku();
        } else if (pil == 5) { lihatRiwayatPembelian(); continue; }

        if (pil >= 2 && pil <= 4) {
            cout << "Masukkan No Rak & Slot: "; cin >> r >> s; r--; s--;
            if (r < 0 || r >= MAX_RAK || s < 0 || s >= MAX_SLOT) { 
                cout << "Koordinat di luar batas!\n"; 
                aksiSelesai(); 
                continue; 
            }
            cin.ignore(); 
        }
    
        if (pil == 2) {
            tambahbuku(r, s);
        } else if (pil == 3 && rakBuku[r][s].isTersedia) {
            tambahstok(r, s);
        } else if (pil == 4 && rakBuku[r][s].isTersedia) {
            rakBuku[r][s].isTersedia = false;
            saveDatabaseBuku();
            cout << "Buku dihapus.\n";
        }
        aksiSelesai();
    }while(pil != 6);
}

void halamanUser(float d) {
    int pil;
    do{
        cout << "\n=== TOKO BUKU SINAR ABADI ===\n"
             << "1. Daftar Semua Buku\n"
             << "2. Cari via Kategori\n"
             << "3. Cari Judul\n"
             << "4. Best Seller\n"
             << "5. Beli Buku\n"
             << "6. Exit\n"
             << "Pilih: "; cin >> pil;
        system("cls");
        if (pil == 1) daftarbuku();
        else if (pil == 2) kategoribuku();
        else if (pil == 3) cariBukuJudul();
        else if (pil == 4) sortBukuBestSeller();
        else if (pil == 5) beliBuku(d);
        if (pil != 6 && pil != 2) aksiSelesai();
    }while(pil != 6);
}

void buatMember() {
    member member_temp;
    loadDatabaseMember();
    bool sudahAda;
    cout << "=====membuat member======\n";
    do{
        sudahAda = false;
        cout << "silahkan masukkan\n";
        cout << "Username: "; getline(cin, member_temp.username);
        for (int i = 0; i < jumlah_member; i++) {
            if ((member_temp.username == membership[i].username) && membership[i].isTersedia) {
                sudahAda = true;
                break;
            }
        }
        if (sudahAda) {
            cout << "Username sudah tersedia / digunakan orang lain!\n";
        }
    }while(sudahAda);
    
    cout << "Password: "; getline(cin, member_temp.password);
    member_temp.isTersedia = true;
    saveDatabaseMember(member_temp);
}

bool prosesLogin(string type) {
    int maxAttempt = 3; string u, p;
    cin.ignore(); 
    (type == "A" ? loadDatabaseAdmin() : loadDatabaseMember());
    while (maxAttempt > 0) {
        cout << "=== LOGIN " << (type == "A" ? "ADMIN" : "MEMBER") << " ===\n"
             << "Username: "; getline(cin, u);
        cout << "Password: "; getline(cin, p);
        for (int i = 0; i < 100; i++) {
            if ((u == staff[i].username && p == staff[i].password) || (u == membership[i].username && p == membership[i].password)) {
                cout << "Login Berhasil!\n"; aksiSelesai();
                if (type == "A") halamanAdmin();
                else {
                   float diskon = 0.05;
                   halamanUser(diskon);
                }
                return true;
            }
        }
        maxAttempt--;
        cout << "Salah! Sisa percobaan: " << maxAttempt << "\n";
        aksiSelesai();
        cout << "\n";
    }
    return false;
}

int main() {
    loadDatabaseBuku();
    int pil;
    do{
        cout << "=== MENU UTAMA ===\n"
             << "1. Pelanggan / Member\n"
             << "2. Petugas Admin\n"
             << "3. Keluar\n"
             << "Pilih: "; 
        
        if (!(cin >> pil)) { 
            cin.clear(); 
            cin.ignore();
            system("cls");
            continue;
        }
        
        system("cls");
        if (pil == 3) { cout << "Sampai jumpa!\n"; break; }
        
        if (pil == 1) {
            char ans;
            cout << "Masuk akun membership (y/n)? "; cin >> ans; system("cls");
            if (ans == 'y' || ans == 'Y') prosesLogin("U");
            else {
                char create;
                cout << "Mau membuat akun membership(y/n)? "; cin >> create;
                cin.ignore(); 
                if (create == 'y' || create == 'Y') {
                    buatMember();
                    float diskon = 0.05;
                    halamanUser(diskon);
                }else {
                    float diskon = 1;
                    halamanUser(diskon);
                }
            }
        } else if (pil == 2) prosesLogin("A");
    }while(pil != 3);
    return 0;
}
