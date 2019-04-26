# SoalShift_modul4_B10

## **Soal Nomor 1**
Semua nama file dan folder harus terenkripsi
Enkripsi yang Atta inginkan sangat sederhana, yaitu Caesar cipher. Namun, Kusuma mengatakan enkripsi tersebut sangat mudah dipecahkan. Dia menyarankan untuk character list diekspansi,tidak hanya alfabet, dan diacak. Berikut character list yang dipakai:
```
qE1~ YMUR2"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\8s;g<{3.u*W-0
```
Misalkan ada file bernama “halo” di dalam folder “INI_FOLDER”, dan key yang dipakai adalah 17, maka:
**“INI_FOLDER/halo”** saat belum di-mount maka akan bernama **“n,nsbZ]wio/QBE#”**, saat telah di-mount maka akan otomatis terdekripsi kembali menjadi **“INI_FOLDER/halo”** .
Perhatian: Karakter ‘/’ adalah karakter ilegal dalam penamaan file atau folder dalam *NIX, maka dari itu dapat diabaikan

### Penyelesaian

- Kode untuk melakukan enkripsi dan dekripsi

    ```c
    void encode(char *str) {
        if (!strcmp(str, ".")) return;
        if (!strcmp(str, "..")) return;
        
        int len = strlen(str), i, j;
        for(i=0; i<len; i++) 
        {
            char tmp = str[i];
            for(j=0; j<94; j++) {
                str[i] = (tmp == chyper[j]? chyper[(j+KEY)%94] : str[i]);
            }
        }
    }

    void decode(char *str) {
        if (!strcmp(str, ".")) return;
        if (!strcmp(str, "..")) return;
        
        int len = strlen(str), i, j;
        for(i=0; i<len; i++)
        {
            char tmp = str[i];
            for(j=0; j<94; j++) {
                str[i] = (tmp == chyper[j]? chyper[(j+94-KEY)%94] : str[i]);
            }
        }
    }
    ```

- Untuk melakukan dekripsi dari path asli (shift4) dapat menggunakan temporary array of char untuk menyimpan path yang ada di folder mounted point kemudian dienkripsi.

    ```c
    sprintf(new_name, "%s", path);
    encode(new_name);

	sprintf(fpath,"%s%s",dirpath,new_name);
    ```

- Ketika menampilkan nama file di layar, hasilnya di dekripsi ulang agar bisa tampil sesuai namanya.

    ```c
    char tmp[11511]; // Used to store the decoded string
    char tmp2[11511]; // Used to 
    struct stat st;
    struct stat tmstat;
    /* --------------------------------- */
    memset(&st, 0, sizeof(st));
    st.st_ino = de->d_ino;
    st.st_mode = de->d_type << 12;

    strcpy(tmp, de->d_name);
    sprintf(tmp2, "%s/%s", fpath,tmp);
    decode(tmp);
    ```

- Teknik ini dilakukan pada hampir semua operasi yang dibutuhkan.

## **Soal Nomor 2**

### Penyelesaian

## **Soal Nomor 3**
Sebelum diterapkannya file system ini, Atta pernah diserang oleh hacker LAPTOP_RUSAK yang menanamkan user bernama “chipset” dan “ic_controller” serta group “rusak” yang tidak bisa dihapus. Karena paranoid, Atta menerapkan aturan pada file system ini untuk menghapus “file bahaya” yang memiliki spesifikasi:
  - Owner Name     : ‘chipset’ atau ‘ic_controller’
  - Group Name    : ‘rusak’
  - Tidak dapat dibaca
  
Jika ditemukan file dengan spesifikasi tersebut ketika membuka direktori, Atta akan menyimpan nama file, group ID, owner ID, dan waktu terakhir diakses dalam file “filemiris.txt” (format waktu bebas, namun harus memiliki jam menit detik dan tanggal) lalu menghapus “file bahaya” tersebut untuk mencegah serangan lanjutan dari LAPTOP_RUSAK.

### Penyelesaian

- Ketika hendak membaca isi folder, maka pertama kali adalah melakukan pengecekkan apakah terdapat file yang memiliki username "chipset" atau "ic_controller" dengan grup "rusak".

    ```c
    stat(tmp2, &tmstat);
    struct passwd *username = getpwuid(tmstat.st_uid);
    struct group *grup = getgrgid(tmstat.st_gid);

    printf("%s %s\n",username->pw_name, grup->gr_name);

    int isUsernameChipset = strcmp(username->pw_name, "chipset");
    int isUsernameIc = strcmp(username->pw_name, "ic_controller");
    int checkGroup = strcmp(grup->gr_name, "rusak");

    isUsernameChipset = (isUsernameChipset==0 ? TRUE : FALSE);
    isUsernameIc = (isUsernameIc==0 ? TRUE : FALSE);
    checkGroup = (checkGroup==0 ? TRUE : FALSE);

    FILE *f_pointer;
    char to_miris[1221], time_str[1221], buffer[1221];

    if ((isUsernameChipset==TRUE || isUsernameIc==TRUE) && checkGroup==TRUE && !(tmstat.st_mode & 0444))
    {
        printf("%d %d %d\n",isUsernameChipset, isUsernameIc, checkGroup);
        char filemiris[1000];

        strcpy(to_miris, dirpath);
        strcat(filemiris, "/filemiris.txt");
        encode(filemiris);

        strcat(to_miris, filemiris);

        f_pointer = fopen(to_miris, "a+"); // a+ untuk append //
        time_t now_time = time(NULL);


        strftime(time_str, 50, "%H:%M:%S %D-%m-%Y", localtime(&tmstat.st_atime));
        fprintf(f_pointer, "%s\t%d:%d\t%s\t%s\n", time_str, username->pw_uid, grup->gr_gid, path, tmp);
        fclose(f_pointer);
        remove(tmp2);

    }
    ```

- Dalam melakukan pengecekkan, jika memang benar bahwa terdapat file yang memiliki username dan group pada poin 1, maka kita menyiapkan filemiris.txt.

    ```c
    char filemiris[1000];

    strcpy(to_miris, dirpath);
    strcat(filemiris, "/filemiris.txt");
    encode(filemiris);

    strcat(to_miris, filemiris);
    ```

- Lalu menyiapkan format waktu (yang berisi tanggal dan waktu).

    ```c
    strftime(time_str, 50, "%H:%M:%S %D-%m-%Y", localtime(&tmstat.st_atime));
    ```

- Kemudian meng-append format waktu yang telah dipersiapkan sebelumnya dan menghapus file tadi.

    ```c
    strftime(time_str, 50, "%H:%M:%S %D-%m-%Y", localtime(&tmstat.st_atime));
    fprintf(f_pointer, "%s\t%d:%d\t%s\t%s\n", time_str, username->pw_uid, grup->gr_gid, path, tmp);
    fclose(f_pointer);
    remove(tmp2);
    ```


## **Soal Nomor 4**
Pada folder **YOUTUBER**, setiap membuat folder permission foldernya akan otomatis menjadi 750. Juga ketika membuat file permissionnya akan otomatis menjadi 640 dan ekstensi filenya akan bertambah “.**iz1**”. File berekstensi “.**iz1**” tidak bisa diubah permissionnya dan memunculkan error bertuliskan “File ekstensi iz1 tidak boleh diubah permissionnya.”

### Penyelesaian


## **Soal Nomor 5**
Ketika mengedit suatu file dan melakukan save, maka akan terbuat folder baru bernama **Backup** kemudian hasil dari save tersebut akan disimpan pada backup dengan nama **namafile_[timestamp].ekstensi**. Dan ketika file asli dihapus, maka akan dibuat folder bernama **RecycleBin**, kemudian file yang dihapus beserta semua backup dari file yang dihapus tersebut (jika ada) di zip dengan nama **namafile_deleted_[timestamp].zip** dan ditaruh ke dalam folder RecycleBin (file asli dan backup terhapus). Dengan format **[timestamp]** adalah **yyyy-MM-dd_HH:mm:ss**

### Penyelesaian
