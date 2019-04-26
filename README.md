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

## **Soal Nomor 2**

### Penyelesaian

## **Soal Nomor 3**
Sebelum diterapkannya file system ini, Atta pernah diserang oleh hacker LAPTOP_RUSAK yang menanamkan user bernama “chipset” dan “ic_controller” serta group “rusak” yang tidak bisa dihapus. Karena paranoid, Atta menerapkan aturan pada file system ini untuk menghapus “file bahaya” yang memiliki spesifikasi:
  - Owner Name     : ‘chipset’ atau ‘ic_controller’
  - Group Name    : ‘rusak’
  - Tidak dapat dibaca
  
Jika ditemukan file dengan spesifikasi tersebut ketika membuka direktori, Atta akan menyimpan nama file, group ID, owner ID, dan waktu terakhir diakses dalam file “filemiris.txt” (format waktu bebas, namun harus memiliki jam menit detik dan tanggal) lalu menghapus “file bahaya” tersebut untuk mencegah serangan lanjutan dari LAPTOP_RUSAK.

### Penyelesaian


## **Soal Nomor 4**
Pada folder **YOUTUBER**, setiap membuat folder permission foldernya akan otomatis menjadi 750. Juga ketika membuat file permissionnya akan otomatis menjadi 640 dan ekstensi filenya akan bertambah “.**iz1**”. File berekstensi “.**iz1**” tidak bisa diubah permissionnya dan memunculkan error bertuliskan “File ekstensi iz1 tidak boleh diubah permissionnya.”

### Penyelesaian


## **Soal Nomor 5**
Ketika mengedit suatu file dan melakukan save, maka akan terbuat folder baru bernama **Backup** kemudian hasil dari save tersebut akan disimpan pada backup dengan nama **namafile_[timestamp].ekstensi**. Dan ketika file asli dihapus, maka akan dibuat folder bernama **RecycleBin**, kemudian file yang dihapus beserta semua backup dari file yang dihapus tersebut (jika ada) di zip dengan nama **namafile_deleted_[timestamp].zip** dan ditaruh ke dalam folder RecycleBin (file asli dan backup terhapus). Dengan format **[timestamp]** adalah **yyyy-MM-dd_HH:mm:ss**

### Penyelesaian
