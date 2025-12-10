# Project-Object-3D-FIP-Kelompok-8
# 🏛️ Simulasi 3D Kompleks Gedung FIP UNESA

Proyek ini adalah implementasi **Grafika Komputer 3D** yang memvisualisasikan kompleks **Fakultas Ilmu Pendidikan (FIP) Universitas Negeri Surabaya (UNESA)**. 

Dibangun menggunakan bahasa **C++** dan pustaka **OpenGL (FreeGLUT)**, proyek ini mensimulasikan lingkungan kampus secara prosedural menggunakan primitif dasar geometri, transformasi hierarkis (*Hierarchical Modeling*), dan teknik pencahayaan (*Lighting*).

## 👥 Anggota Kelompok

Berikut adalah kontributor yang menyusun proyek ini:

1.  **YUSUF RAMADHAN MORTI SONDANG SIMANJUNTAK** - [24050974067]
2.  **RINA DWI APRI LESTARI** - [24050974076]
3.  **ALFIAN WIDHI HARJO** - [24050974087]

## 🏫 Deskripsi Proyek

Simulasi ini menampilkan tata letak realistis dari 6 gedung utama di area FIP UNESA. Setiap gedung dirancang dengan detail arsitektur yang mendekati aslinya, termasuk penggunaan atap model Joglo, pilar-pilar penyangga, dan kombinasi warna identitas fakultas (Krem, Coklat, Ungu).

### Gedung yang Divisualisasikan:

| Gedung | Deskripsi & Lokasi |
| :--- | :--- |
| **Gedung 03** | Gedung lama yang terletak di sisi **kiri depan**. |
| **Gedung 04** | Gedung lama yang terletak di sisi **kanan depan**. |
| **Gedung L** | Gedung baru berbentuk L yang terletak di **depan Gedung 04**. |
| **Gedung 06** | Gedung mewah 4 lantai dengan atap Joglo besar dan pilar berwarna coklat kemerahan. Terletak di **belakang Gedung 03** (menghadap belakang). |
| **Gedung ULABK** | Gedung laboratorium yang terletak di **belakang Gedung 04** (menghadap belakang). |
| **Gedung 02** | Gedung dengan desain modern yang terletak di **samping kanan Gedung L**. |

## ✨ Fitur Utama

* **First-Person Camera:** Navigasi kamera interaktif yang memungkinkan pengguna "berjalan" mengelilingi dan melihat seluruh sudut kampus.
* **Mouse Look:** Kemampuan menoleh ke segala arah (atas, bawah, kiri, kanan) menggunakan pergerakan mouse.
* **Procedural Modeling:** Gedung dibangun menggunakan fungsi `drawBalok` kustom dan transformasi matriks (`glPushMatrix`/`glPopMatrix`), tanpa memuat model 3D eksternal (.obj/.fbx).
* **Detail Arsitektur:** Implementasi kusen jendela, pintu, pilar memutar, atap limasan/joglo, dan plakat nama gedung.
* **Pencahayaan & Material:** Penggunaan `GL_LIGHTING` dan `glMaterial` untuk memberikan efek kedalaman, bayangan, dan nuansa 3D yang nyata pada objek.

## 🎮 Kontrol Navigasi

Gunakan kombinasi Keyboard dan Mouse untuk berinteraksi:

| Tombol / Aksi | Fungsi |
| :--- | :--- |
| **W** | Maju ke depan |
| **S** | Mundur ke belakang |
| **A** | Geser ke kiri (*Strafe Left*) |
| **D** | Geser ke kanan (*Strafe Right*) |
| **Spasi** | Terbang Naik (*Fly Up*) |
| **C** | Terbang Turun (*Fly Down*) |
| **Gerakan Mouse** | Menoleh / Mengarahkan pandangan |
| **ESC** | Keluar dari program |

## 🛠️ Teknologi & Tools

* **Bahasa Pemrograman:** C++
* **Grafis API:** OpenGL (Legacy Pipeline)
* **Library:** FreeGLUT / GLUT
* **IDE:** Visual Studio / Code::Blocks / VS Code

---
**Tugas Akhir Praktikum Grafika Komputer** *Teknik Informatika - Universitas Negeri Surabaya*
