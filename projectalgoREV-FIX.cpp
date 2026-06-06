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
const string File_Buku = ("data/DatabaseBuku.txt");
const string File_Riwayat = ("data/DatabaseRiwayat.txt");
const string File_Member = ("data/DatabaseMember.txt");
const string File_Admin = ("data/DatabaseAdmin.txt");
buku rakBuku[MAX_RAK * MAX_SLOT];
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

bool cek_input(string input, int& hasil) {
    if (input.empty()) return false;
    
    stringstream ss(input);
    int angka;
    char sisa;
    if (ss >> angka) {
        if (ss >> sisa) {
            return false;
        }
        hasil = angka;
        return true;
    }
    return false;
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
        getline(ss, temp); if (!temp.empty()) buku_temp.isTersedia = (temp == "1" ? true : false);

        buku_temp.namaBuku = replaceUnderscoretoSpace(buku_temp.namaBuku);
        buku_temp.namaPenulis = replaceUnderscoretoSpace(buku_temp.namaPenulis);
        buku_temp.kategori = replaceUnderscoretoSpace(buku_temp.kategori);

        rakBuku[((buku_temp.rak - 1) * MAX_SLOT) + (buku_temp.slot - 1)] = buku_temp;
    }
    file.close();
}

void saveDatabaseBuku() {
    ofstream file(File_Buku, ios::trunc);
    if (!file.is_open()) return;
    for (int i = 0; i < MAX_RAK * MAX_SLOT; i++) {
        int nomorRak = (i / MAX_SLOT) + 1;
        int nomorSlot = (i % MAX_SLOT) + 1;
        if (rakBuku[i].isTersedia) {
            file << nomorRak<< " " << nomorSlot << " " << replaceSpacetoUnderscore(rakBuku[i].namaBuku)
                 << " " << replaceSpacetoUnderscore(rakBuku[i].namaPenulis)
                 << " " << rakBuku[i].tahunTerbit << " " << replaceSpacetoUnderscore(rakBuku[i].kategori)
                 << " " << rakBuku[i].harga << " "<< rakBuku[i].jumlahBest << " " << rakBuku[i].jumlahStok
                 << " " << rakBuku[i].isTersedia << "\n";
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
        getline(ss, temp); if (!temp.empty()) pelanggan_temp.jumlahBeli = stoi(temp);
        
        pelanggan_temp.namaPelanggan = replaceUnderscoretoSpace(pelanggan_temp.namaPelanggan);
        pelanggan_temp.namaBukuDibeli = replaceUnderscoretoSpace(pelanggan_temp.namaBukuDibeli);
        cout << "Pelanggan: " << pelanggan_temp.namaPelanggan << "\n"
             << "Tgl: " << pelanggan_temp.tanggal << "\n"
             << "Buku: " << pelanggan_temp.namaBukuDibeli << "\n"
             << "Jml: " << pelanggan_temp.jumlahBeli << " pcs\n---------------------\n";
        ada = true;
    }
    if (!ada) cout << "[ Belum ada data transaksi ]\n";
    file.close();
}

void saveTransaksi(pelanggan p) {
    ofstream file(File_Riwayat, ios::app);
    if (!file.is_open()) return;

    file << replaceSpacetoUnderscore(p.namaPelanggan) << " " << replaceSpacetoUnderscore(p.tanggal) << " "
         << replaceSpacetoUnderscore(p.namaBukuDibeli) << " " << p.jumlahBeli << "\n";
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
        getline(ss, temp); if (!temp.empty()) member_temp.isTersedia = (temp == "1" ? true : false);

        member_temp.username = replaceUnderscoretoSpace(member_temp.username);
        member_temp.password = replaceUnderscoretoSpace(member_temp.password);
        membership[jumlah_member] = member_temp;
        jumlah_member++;
    }
    file.close();
}

void saveDatabaseMember(member m) {
    ofstream file(File_Member, ios::app);
    if (!file.is_open()) return;
    file << replaceSpacetoUnderscore(m.username) << " " << replaceSpacetoUnderscore(m.password)
         << " " << m.isTersedia << "\n";
    file.close();
    
    cout << "berhasil kamu langsung masuk sebagai membership\n";
}

void saveAllMembers() {
    ofstream file(File_Member, ios::trunc); 
    if (!file.is_open()) return;
    
    for (int i = 0; i < jumlah_member; i++) {
        file << replaceSpacetoUnderscore(membership[i].username) << " " 
             << replaceSpacetoUnderscore(membership[i].password) << " " 
             << membership[i].isTersedia << "\n";
    }
    file.close();
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
        getline(ss, admin_temp.password);

        admin_temp.username = replaceUnderscoretoSpace(admin_temp.username);
        admin_temp.password = replaceUnderscoretoSpace(admin_temp.password);
        staff[jumlah_admin] = admin_temp;
        jumlah_admin++;
    }
    file.close();
}

void sortBukuBestSeller() {
    buku* daftarPointer[MAX_RAK * MAX_SLOT]; int jml = 0;
    for (int i = 0; i < MAX_RAK * MAX_SLOT; i++)
        if (rakBuku[i].isTersedia) daftarPointer[jml++] = &rakBuku[i];

    if (jml == 0) { cout << "Belum ada data buku!\n"; aksiSelesai(); return; }

    for (int i = 0; i < jml - 1; i++)
        for (int j = 0; j < jml - i - 1; j++)
            if (daftarPointer[j]->jumlahBest < daftarPointer[j+1]->jumlahBest) {
                buku* temp = daftarPointer[j];
                daftarPointer[j] = daftarPointer[j+1];
                daftarPointer[j+1] = temp;
            }
            
    cout << "=== DAFTAR BUKU BEST SELLER ===\n";
    for (int i = 0; i < jml; i++)
        cout << "Judul: " << daftarPointer[i]->namaBuku << "\n"
             << "Penulis: " << daftarPointer[i]->namaPenulis << "\n"
             << "Tahun Terbit: " << daftarPointer[i]->tahunTerbit << "\n"
             << "Kategori: " << daftarPointer[i]->kategori << "\n"
             << "Harga: " << daftarPointer[i]->harga << "\n"
             << "Terjual: " << daftarPointer[i]->jumlahBest << " pcs\n"
             << "Sisa: " << daftarPointer[i]->jumlahStok << "\n---------------------\n";
}

void daftarbuku() {
    cout << "=== DAFTAR BUKU ===\n\n"; bool ada = false;
    for (int i = 0; i < MAX_RAK * MAX_SLOT; i++) {
        int nomorRak = (i / MAX_SLOT) + 1;
        int nomorSlot = (i % MAX_SLOT) + 1;
        if (rakBuku[i].isTersedia) {
            cout << " -> [Rak " << nomorRak << ", Slot " << nomorSlot << "] " << "\n"
                 << "Nama Buku: " << rakBuku[i].namaBuku << "\n"
                 << "Penulis: " << rakBuku[i].namaPenulis << "\n"
                 << "Tahun Terbit: " << rakBuku[i].tahunTerbit << "\n"
                 << "Kategori: " << rakBuku[i].kategori << "\n"
                 << "Harga: " << rakBuku[i].harga << "\n"
                 << "Stok: " << rakBuku[i].jumlahStok << " pcs\n\n";
            ada = true;
        }
    }
    if (!ada) cout << "[ Seluruh Rak Buku Masih Kosong ]\n";
}

void cariBukuJudul() {
    string keyword;
    cout << "=== CARI JUDUL BUKU ===\n"
         << "Masukkan Judul: ";
    getline(cin, keyword);
    
    bool f = false;
    for (int i = 0; i < MAX_RAK * MAX_SLOT; i++) {
        buku* ptrBuku = &rakBuku[i];
        int nomorRak = (i / MAX_SLOT) + 1;
        int nomorSlot = (i % MAX_SLOT) + 1;
        if ((ptrBuku->isTersedia) && ptrBuku->namaBuku == keyword) {
            cout << " -> [Rak " << nomorRak << ", Slot " << nomorSlot << "]\n"
                 << "Nama Buku: " << ptrBuku->namaBuku << "\n"
                 << "Penulis: " << ptrBuku->namaPenulis << "\n"
                 << "Tahun Terbit: " << ptrBuku->tahunTerbit << "\n"
                 << "Kategori: " << ptrBuku->kategori << "\n"
                 << "Harga: " << ptrBuku->harga << "\n"
                 << "Stok: " << ptrBuku->jumlahStok << " pcs\n";
                f = true;
            }
    }
    if (!f) cout << "Buku \"" << keyword << "\" tidak ditemukan.\n";
}

void beliBuku(float diskon) {
    string l, k, j;
    int r, s, jmlBeli;
    pelanggan p;
    bool koordinatValid = false;
    do {
        cout << "=== BELI BUKU ===\n"
             << "Masukkan No Rak & Slot (misal: 1 2): "; 
        cin >> l >> k;
        cin.ignore(1000, '\n'); 

        if(!cek_input(l, r) || !cek_input(k, s)) {
            cout << "Input rak dan slot harus berupa angka valid!\n\n";
            r = 0; s = 0;
            continue;
        }
        if (r >=1 && r < MAX_RAK && s >= 1 && s < MAX_SLOT) {
            koordinatValid = true;
        } else {
            cout << "Nomor rak atau slot di luar batas jangkauan! Silahkan coba lagi.\n\n";
        }

    } while (!koordinatValid);
    int indeks = ((r - 1) * MAX_SLOT) + (s - 1);
    if (rakBuku[indeks].isTersedia && rakBuku[indeks].jumlahStok > 0) {
        cout << "\nBuku Terpilih:\n"
             << "Nama Buku      : " << rakBuku[indeks].namaBuku << "\n" 
             << "Harga (perbuku): " << rakBuku[indeks].harga - (rakBuku[indeks].harga * diskon) << "\n"
             << "Stok Tersedia  : " << rakBuku[indeks].jumlahStok << " pcs\n";
        do {
            cout << "Jumlah Beli: "; cin >> j;
            cin.ignore(1000, '\n'); 
            
            if (!cek_input(j, jmlBeli)) {
                cout << "Input jumlah harus berupa angka bulat!\n\n";
                jmlBeli = 0; 
                continue;
            }
            if (jmlBeli <= 0 || jmlBeli > rakBuku[indeks].jumlahStok) {
                cout << "Jumlah tidak valid atau stok buku tidak mencukupi!\n\n";
                jmlBeli = 0; 
            }
        } while (jmlBeli <= 0);

        cout << "Nama Anda: "; getline(cin, p.namaPelanggan);
        cout << "Tanggal (DD-MM-YYYY): "; getline(cin, p.tanggal);
        rakBuku[indeks].jumlahStok -= jmlBeli;
        if (rakBuku[indeks].jumlahStok == 0){
            rakBuku[indeks].isTersedia = false;
        } 
        rakBuku[indeks].jumlahBest += jmlBeli;
        p.namaBukuDibeli = rakBuku[indeks].namaBuku; 
        p.jumlahBeli = jmlBeli;

        saveDatabaseBuku(); 
        saveTransaksi(p);
        cout << "\nPembelian Berhasil!\n";
    } else {
        cout << "Maaf, buku di lokasi Rak " << r << " Slot " << s << " sedang kosong atau tidak tersedia!\n";
    }
}

void kategoribuku() {
    string pil;
    int pilAngka;
    do{
        cout << "=== KATEGORI BUKU ===\n"
             << "1. Fiksi\n"
             << "2. Non-Fiksi\n"
             << "3. Kembali\n"
             << "Pilih: "; cin >> pil;
        cin.ignore(1000, '\n'); 

        if(!cek_input(pil, pilAngka)) {
            pilAngka = 0;
            cout << "Input harus berupa angka valid\n";
            aksiSelesai(); continue;
        }
        if (pilAngka == 3) {
            continue; 
        }
        if (pilAngka < 1 || pilAngka > 2) {
            cout << "Pilihan menu tidak tersedia!\n";
            aksiSelesai();
            continue;
        }
        string katTarget = (pilAngka == 1) ? "Fiksi" : "Non-Fiksi";
        string katTargetLower = (pilAngka == 1) ? "fiksi" : "non-fiksi";
        cout << "=== " << katTarget << " ===\n";
        bool ketemu = false;
        for (int i = 0; i < MAX_RAK * MAX_SLOT; i++) {
            int nomorRak = (i / MAX_SLOT) + 1;
            int nomorSlot = (i % MAX_SLOT) + 1;
            if (rakBuku[i].isTersedia && (rakBuku[i].kategori == katTarget || rakBuku[i].kategori == katTargetLower)) {
                cout << " -> [Rak " << nomorRak << "][Slot " << nomorSlot << "] " << "\n"
                     << "Nama Buku    : " << rakBuku[i].namaBuku << "\n" 
                     << "Harga (Diskon): " << rakBuku[i].harga << "\n"
                     << "Stok Tersedia: " << rakBuku[i].jumlahStok << " pcs\n---------------------\n";
                ketemu = true;
            }
        }
        if (!ketemu) cout << "Kategori ini belum tersedia.\n";
        aksiSelesai();
    }while(pilAngka != 3);
}

void tambahbuku(int rak, int slot) {
    string temp, kat;
    int sub;
    int indeks = -1;
    for (int i = 0; i < MAX_RAK * MAX_SLOT; i++) {
        if (!rakBuku[i].isTersedia) {
            indeks = i;
            break;
        }
    }
    if (indeks == -1) {
        cout << "Maaf, seluruh rak dan slot buku sudah penuh!\n";
        return;
    }
    int nomorRak = (indeks / MAX_SLOT) + 1;
    int nomorSlot = (indeks % MAX_SLOT) + 1;

    cout << "=== TAMBAH BUKU (Otomatis) ===\n";
    cout << "Lokasi Buku: Rak " << nomorRak << ", Slot " << nomorSlot << "\n";
    cout << "-------------------------------------------\n";
    string buku;
    bool sudahAda;
    do{
        sudahAda = false;
        cout << "Judul: "; getline(cin, buku);
        for (int i = 0; i < (MAX_RAK * MAX_SLOT); i++) {
            if (buku == rakBuku[i].namaBuku){
                sudahAda = true;
                break;
            }
        }
    }while (sudahAda);
    rakBuku[indeks].namaBuku = buku;
    cout << "Penulis: "; getline(cin, rakBuku[indeks].namaPenulis);

    do{
        cout << "Tahun Terbit: "; cin >> temp;
        cin.ignore(1000, '\n'); 
        if(!cek_input(temp, sub)) {
        sub = 0;
        cout << "Input harus berupa angka valid\n";
        }
    }while(sub == 0);
    rakBuku[indeks].tahunTerbit = sub;
        
    do {
        cout << "Kategori (Fiksi/Non-Fiksi): "; getline(cin, kat);
    }while(kat != "fiksi" && kat != "Fiksi" && kat != "non-fiksi" && kat != "Non-Fiksi"); 
        
    rakBuku[indeks].kategori = kat;
    do{
        sub = 0;
        cout << "Harga: "; cin >> temp;
        cin.ignore(1000, '\n');
        if(!cek_input(temp, sub)) {
            sub = 0;
            cout << "Input harus berupa angka valid\n";
        }
    }while(sub == 0);
    rakBuku[indeks].harga = sub;

    do{
        sub = 0;
        cout << "Stok: "; cin >> temp;
        cin.ignore(1000, '\n');
        if(!cek_input(temp, sub)) {
            sub = 0;
            cout << "Input harus berupa angka valid\n";
        }
    }while(sub == 0);
    rakBuku[indeks].jumlahStok = sub;

    rakBuku[indeks].jumlahBest = 0;
    rakBuku[indeks].isTersedia = true;
    saveDatabaseBuku();
    cout << "\nBuku berhasil didaftarkan!\n";
}


void tambahstok(int rak, int slot) {
    string tam;
    int tamAngka;
    int indeks = ((rak - 1) * MAX_SLOT) + slot - 1;
    cout << "Nama Buku: " << rakBuku[indeks].namaBuku << "\n";
    cout << "Stok lama: " << rakBuku[indeks].jumlahStok << "\n";
    do{
        cout << "Tambahan Stok: "; cin >> tam;
        cin.ignore(1000, '\n');
    
        if(!cek_input(tam, tamAngka)) {
            tamAngka = 0;
            cout << "Jumlah tidak valid atau stok kurang\n";
        }
    }while(tamAngka <= 0);
    rakBuku[indeks].jumlahStok += tamAngka;
    saveDatabaseBuku();
    cout << "Stok diperbarui!\n";
}

void hapusBuku(int ind) {
    for (int j = ind; j < (MAX_RAK * MAX_SLOT) - 1; j++) {
        if (rakBuku[j].isTersedia) {
            rakBuku[j] = rakBuku[j + 1];
        }
    }
    saveDatabaseBuku();
    cout << "sudah dihapus.\n";
}

void kelolaMembership() {
    string pil;
    int pilAngka;
    do {
        loadDatabaseMember();
        cout << "=== KELOLA MEMBERSHIP ===\n"
             << "1. Daftar Semua Member\n"
             << "2. Nonaktifkan Member\n"
             << "3. Hapus Member Permanen\n"
             << "4. Kembali ke Menu Admin\n"
             << "Pilih: "; getline(cin, pil);
        system("cls");

        if(!cek_input(pil, pilAngka)) {
            pilAngka = 0;
            cout << "Input harus berupa angka valid\n";
            aksiSelesai(); continue;
        }

        if (pilAngka == 1) {
            cout << "=== DAFTAR MEMBER ===\n";
            if (jumlah_member == 0) cout << "[ Belum ada data member ]\n";
            for (int i = 0; i < jumlah_member; i++) {
                cout << "Username: " << membership[i].username << "\n"
                     << "Status: " << (membership[i].isTersedia ? "Aktif" : "Nonaktif") << "\n\n";
            } aksiSelesai(); continue;
        } 
        else if (pilAngka == 2) {
            string target;
            cout << "=== NONAKTIFKAN MEMBER ===\n"
                 << "Masukkan Username Member: "; getline(cin, target);
            
            bool ketemu = false;
            for (int i = 0; i < jumlah_member; i++) {
                if (membership[i].username == target) {
                    if (!membership[i].isTersedia) {
                        cout << "Member memang sudah dalam keadaan nonaktif.\n";
                    } else {
                        membership[i].isTersedia = false;
                        saveAllMembers();
                        cout << "Member \"" << target << "\" berhasil dinonaktifkan!\n";
                    }
                    ketemu = true;
                    break;
                }
            }
            if (!ketemu) cout << "Username tidak ditemukan.\n";
            aksiSelesai(); continue;
        } 
        else if (pilAngka == 3) {
            string target;
            cout << "=== HAPUS MEMBER PERMANEN ===\n"
                 << "Masukkan Username Member yang akan dihapus: "; getline(cin, target);
            
            bool ketemu = false;
            for (int i = 0; i < jumlah_member; i++) {
                if (membership[i].username == target) {
                    for (int j = i; j < jumlah_member - 1; j++) {
                        membership[j] = membership[j + 1];
                    }
                    jumlah_member--;
                    saveAllMembers();
                    cout << "Member \"" << target << "\" berhasil dihapus secara permanen.\n";
                    ketemu = true;
                    break;
                }
            }
            if (!ketemu) cout << "Username tidak ditemukan.\n";
        }
        aksiSelesai();
    } while (pilAngka != 4);
}

void halamanAdmin() {
    string pil;
    int pilAngka, r, s;
    do{
        cout << "=== MENU ADMIN ===\n"
             << "1. Daftar Semua Buku\n"
             << "2. Tambah Buku\n"
             << "3. Tambah Stok\n"
             << "4. Hapus Buku\n"
             << "5. Riwayat Transaksi\n"
             << "6. Kelola Member\n"
             << "7. Keluar\n"
             << "Pilih: "; getline(cin, pil);
        system("cls");

        if(!cek_input(pil, pilAngka)) {
            pilAngka = 0;
            cout << "Input harus berupa angka valid\n";
            aksiSelesai();
            continue;
        }
        if (pilAngka < 1 || pilAngka > 7) {
            cout << "Pilihan menu tidak tersedia!\n";
            aksiSelesai();
            continue;
        }
        if (pilAngka == 1) {
            daftarbuku();
        } else if (pilAngka == 5) {
            lihatRiwayatPembelian(); aksiSelesai();
        } else if (pilAngka == 6) {
            kelolaMembership();
            continue; 
        }

        if (pilAngka >= 3 && pilAngka <= 4) {
            string l, k;
            cout << "Masukkan No Rak & Slot (misal 1 2): "; cin >> l >> k;
            cin.ignore(1000, '\n'); 
            if(!cek_input(l, r)) r = 0;
            if(!cek_input(k, s)) s = 0;
            if (r == 0 || s == 0){
                cout << "Input rak atau slot harus berupa angka valid\n";
                aksiSelesai(); continue;
            }
            if (r <= 0 || r > MAX_RAK || s <= 0 || s > MAX_SLOT) { cout << "Koordinat di luar batas!\n"; aksiSelesai(); continue; }
        }
        int indeks = ((r - 1) * MAX_SLOT) + (s - 1);
        if (pilAngka == 2) {
            tambahbuku(r, s);
        } else if (pilAngka == 3 && rakBuku[indeks].isTersedia) {
            tambahstok(r,s);
        } else if (pilAngka == 4 && rakBuku[indeks].isTersedia) {
            cout << "Nama Buku: " << rakBuku[indeks].namaBuku << "\n";
            hapusBuku(indeks);
        }
        aksiSelesai();
    }while(pilAngka != 7);
}

void halamanUser(float d) {
    string pil;
    int pilAngka;
    do{
        cout << "\n=== TOKO BUKU SINAR ABADI ===\n"
             << "1. Daftar Semua Buku\n"
             << "2. Cari via Kategori\n"
             << "3. Cari Judul\n"
             << "4. Best Seller\n"
             << "5. Beli Buku\n"
             << "6. Keluar\n"
             << "Pilih: "; getline(cin,pil);
        system("cls");

        if(!cek_input(pil, pilAngka)) {
            pilAngka = 0;
            cout << "Input harus berupa angka valid\n";
            aksiSelesai();
            continue;
        }
        if (pilAngka < 1 || pilAngka > 6) {
            cout << "Pilihan menu tidak tersedia!\n";
            aksiSelesai();
            continue;
        }
        if (pilAngka == 1) daftarbuku();
        else if (pilAngka == 2) kategoribuku();
        else if (pilAngka == 3) cariBukuJudul();
        else if (pilAngka == 4) sortBukuBestSeller();
        else if (pilAngka == 5) beliBuku(d);
        aksiSelesai();
    }while(pilAngka != 6);
    cin.clear();
}

 bool buatMember() {
    member member_temp;
    cout << "=====membuat member======\n";
    loadDatabaseMember();
    bool sudahAda;
    do{
        sudahAda = false;
        cout << "silahkan masukkan\n";
        cout << "Username: "; getline(cin, member_temp.username);
        for (int i = 0; i < jumlah_member; i++) {
            if (member_temp.username == membership[i].username) {
                sudahAda = true;
                break;
            }
        }
        if (sudahAda) {
            cout << "\nUsername sudah tersedia / digunakan orang lain!\n";
        }
    }while(sudahAda);
    
    cout << "Password: "; getline(cin, member_temp.password);
    member_temp.isTersedia = true;
    if (!member_temp.username.empty() && !member_temp.password.empty()) {
        saveDatabaseMember(member_temp);
        return true;
    }else {
        cout << "anda belum terdaftar sebagai membership\n";
        return false;
    }
}

bool prosesLogin(string type, int maxAttempt) {
    string u, p;

    if (maxAttempt == 3) {
        (type == "A" ? loadDatabaseAdmin() : loadDatabaseMember());
    }
    if (maxAttempt <= 0) {
        cout << "Login Gagal! Kesempatan Anda telah habis.\n";
        aksiSelesai();
        return false;
    }

    cout << "=== LOGIN " << (type == "A" ? "ADMIN" : "MEMBER") << " ===\n"
         << "Username: "; getline(cin, u);
    cout << "Password: "; getline(cin, p); 

    if (type == "A") {
        for (int i = 0; i < jumlah_admin; i++) {
            if (u == staff[i].username && p == staff[i].password) {
                cout << "Login Berhasil sebagai Admin!\n"; 
                aksiSelesai();
                halamanAdmin();
                return true;  
            }
        }
    } 
    else {
        for (int i = 0; i < jumlah_member; i++) {
            if (u == membership[i].username && p == membership[i].password) {
                if (membership[i].isTersedia) {
                    cout << "Login Berhasil sebagai Member!\n";
                    cout << "Anda mendapatkan diskon 5%\n";
                    aksiSelesai();
                    float diskon = 0.05;
                    halamanUser(diskon);
                    return true;
                } else {
                    maxAttempt--;
                    cout << "Login gagal!\n";
                    cout << "Karena Anda bukan lagi membership\n";
                    aksiSelesai();
                    return prosesLogin(type, maxAttempt);
                }
            }
        }
    }
    maxAttempt--;
    cout << "Salah! Sisa percobaan: " << maxAttempt << "\n";
    aksiSelesai();
    cout << "\n";
    return prosesLogin(type, maxAttempt);
}

int main() {
    loadDatabaseBuku();
    string pil;
    int pilAngka;
    do{
        cout << "=== MENU UTAMA ===\n"
             << "1. Pelanggan / Member\n"
             << "2. Petugas Admin\n"
             << "3. Keluar\n"
             << "Pilih: "; getline(cin, pil);
        system("cls");

        if(!cek_input(pil, pilAngka)){
            pilAngka = 0;
            cout << "Input harus berupa angka valid\n";
            aksiSelesai();
            continue; 
        }
        if (pilAngka < 1 || pilAngka > 3) {
            cout << "Pilihan menu tidak tersedia!\n";
            aksiSelesai();
            continue;
        }
        if (pilAngka == 3) {
            cout << "Sampai jumpa!\n"; break;
        }
        if (pilAngka == 1) {
            char ans;
            cout << "Masuk akun membership (y/n)? "; cin >> ans; 
            cin.ignore(1000, '\n'); 
            system("cls");
            
            if (ans == 'y' || ans == 'Y') {
                prosesLogin("U", 3);
            }
            else {
                char create;
                cout << "Mau membuat akun membership(y/n)? "; cin >> create;  
                cin.ignore(1000, '\n'); 
                system("cls"); 
                if (create == 'y' || create == 'Y') {
                    if (!buatMember()) {
                        aksiSelesai();
                        main();
                    }else {
                        cout << "Anda mendapatkan diskon 5%\n";
                        aksiSelesai();
                        float diskon = 0.05;
                        halamanUser(diskon);
                    }
                }else {
                    float diskon = 0;
                    halamanUser(diskon);
                }
            }
        } else if (pilAngka == 2) {
            prosesLogin("A", 3);
        }
    }while(pilAngka != 3);
    return 0;
}