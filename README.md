# Game_Setting_Modular_IvanSusendra

## Bahasa Indonesia

Template **Game Setting System** untuk proyek skripsi berbasis **Unreal Engine 5**.
Repositori ini berisi dasar sistem pengaturan game yang dapat dikembangkan lebih lanjut sesuai kebutuhan proyek, seperti pengaturan grafis, audio, kontrol, dan fitur pendukung lainnya.

### Persyaratan

Sebelum menggunakan project ini, pastikan pengguna sudah menginstal:

* **Unreal Engine 5** sesuai versi project
* **Visual Studio 2022 versi terbaru**
* Komponen **Desktop development with C++**
* Komponen pendukung C++ untuk Unreal Engine jika tersedia pada installer Visual Studio

### Plugin Eksternal

Project ini memerlukan plugin eksternal tambahan agar seluruh fitur pengaturan grafis dapat digunakan dengan benar.

Pengguna **wajib mengunduh dan memasang plugin berikut** sesuai dengan versi **Unreal Engine 5.6** yang digunakan oleh project:

* **NVIDIA DLSS Plugin for Unreal Engine 5.6**
* **AMD FSR Plugin for Unreal Engine 5.6**

Pastikan plugin yang digunakan benar-benar sesuai dengan versi engine project.

### Cara Menggunakan

#### 1. Clone atau download repository

Clone repository ini ke komputer lokal, atau download dalam bentuk ZIP lalu extract.

#### 2. Buka folder project

Masuk ke folder project yang berisi file **`.uproject`**.

#### 3. Pasang plugin eksternal

Sebelum membuka project, pengguna harus terlebih dahulu mengunduh dan memasang plugin eksternal yang dibutuhkan, yaitu:

* **DLSS UE 5.6**
* **FSR UE 5.6**

Jika diperlukan, letakkan plugin pada folder **`Plugins`** di dalam project.

#### 4. Generate Visual Studio project files

Pengguna **wajib melakukan generate Visual Studio project files** terlebih dahulu sebelum membuka project C++.

Caranya:

* Klik kanan pada file **`.uproject`**
* Pilih **Generate Visual Studio project files**

Langkah ini diperlukan agar file solution **Visual Studio** terbentuk dengan benar.

#### 5. Buka project di Visual Studio 2022

Setelah proses generate selesai:

* Buka file **`.sln`**
* Gunakan **Visual Studio 2022 versi terbaru**
* Tunggu proses indexing dan konfigurasi selesai

#### 6. Build project

Setelah solution berhasil dibuka:

* Pilih konfigurasi build yang sesuai
* Lakukan **Build** pada project
* Jika build berhasil, project siap dijalankan dari Unreal Engine maupun Visual Studio

### Catatan Penting

* Jangan gunakan versi Visual Studio yang terlalu lama, karena dapat menyebabkan error pada proses generate, build, atau kompatibilitas toolchain C++ Unreal Engine.
* Jika plugin eksternal belum terpasang, beberapa fitur seperti **DLSS** dan **FSR** mungkin tidak tersedia atau dapat menyebabkan error saat membuka maupun build project.
* Jika project gagal dibuka, ulangi langkah berikut:

  * tutup Unreal Engine dan Visual Studio
  * pastikan plugin eksternal sudah terpasang dengan benar
  * klik kanan file **`.uproject`**
  * pilih **Generate Visual Studio project files**
  * lalu build ulang di **Visual Studio 2022 versi terbaru**
* Pastikan seluruh dependency C++ untuk Unreal Engine telah terpasang di Visual Studio Installer.

### Tujuan Repository

Repository ini dibuat sebagai template dan dasar implementasi **game setting system** untuk mendukung kebutuhan pengembangan pada skripsi.
Pengguna dapat mengembangkan sistem ini lebih lanjut untuk menyesuaikan kebutuhan project masing-masing.

### Penutup

Jika project tidak dapat dijalankan pada komputer pengguna, langkah pertama yang harus dicek adalah:

1. Apakah **Visual Studio 2022 versi terbaru** sudah terinstal
2. Apakah plugin eksternal **DLSS UE 5.6** dan **FSR UE 5.6** sudah terpasang
3. Apakah file **Visual Studio project files** sudah di-generate dari file **`.uproject`**
4. Apakah project sudah berhasil di-build tanpa error

---

## English

A **Game Setting System** template for a thesis project based on **Unreal Engine 5**.
This repository provides a foundation for a game settings system that can be further developed according to project needs, such as graphics settings, audio settings, controls, and other supporting features.

### Requirements

Before using this project, make sure the user has installed:

* **Unreal Engine 5** that matches the project version
* **The latest version of Visual Studio 2022**
* The **Desktop development with C++** workload
* Additional C++ components for Unreal Engine if available in the Visual Studio installer

### External Plugins

This project requires additional external plugins so that all graphics settings features can work properly.

Users **must download and install the following plugins** according to the **Unreal Engine 5.6** version used by this project:

* **NVIDIA DLSS Plugin for Unreal Engine 5.6**
* **AMD FSR Plugin for Unreal Engine 5.6**

Make sure the installed plugin versions match the engine version used by the project.

### How to Use

#### 1. Clone or download the repository

Clone this repository to your local computer, or download it as a ZIP file and extract it.

#### 2. Open the project folder

Go to the project folder that contains the **`.uproject`** file.

#### 3. Install external plugins

Before opening the project, users must first download and install the required external plugins:

* **DLSS UE 5.6**
* **FSR UE 5.6**

If needed, place the plugins inside the project's **`Plugins`** folder.

#### 4. Generate Visual Studio project files

The user **must generate the Visual Studio project files** before opening the C++ project.

Steps:

* Right-click the **`.uproject`** file
* Select **Generate Visual Studio project files**

This step is required to properly create the **Visual Studio solution** file.

#### 5. Open the project in Visual Studio 2022

After the generation process is complete:

* Open the **`.sln`** file
* Use **the latest version of Visual Studio 2022**
* Wait until indexing and configuration are completed

#### 6. Build the project

After the solution has been opened successfully:

* Select the appropriate build configuration
* Build the project
* If the build is successful, the project is ready to run from Unreal Engine or Visual Studio

### Important Notes

* Do not use an outdated version of Visual Studio, as it may cause errors during the generate process, build process, or Unreal Engine C++ toolchain compatibility.
* If the external plugins are not installed, some features such as **DLSS** and **FSR** may be unavailable or may cause errors when opening or building the project.
* If the project fails to open, repeat these steps:

  * close Unreal Engine and Visual Studio
  * make sure the required external plugins are installed correctly
  * right-click the **`.uproject`** file
  * select **Generate Visual Studio project files**
  * then rebuild the project using **the latest version of Visual Studio 2022**
* Make sure all required C++ dependencies for Unreal Engine are installed through the Visual Studio Installer.

### Repository Purpose

This repository was created as a template and base implementation of a **game setting system** to support thesis development needs.
Users may further expand this system according to their own project requirements.

### Closing

If the project cannot run on the user’s computer, the first things to check are:

1. Whether **the latest version of Visual Studio 2022** is already installed
2. Whether the external plugins **DLSS UE 5.6** and **FSR UE 5.6** are already installed
3. Whether the **Visual Studio project files** have been generated from the **`.uproject`** file
4. Whether the project has been successfully built without errors

---

## License

This project uses a **custom license**.
You may view and study the source code for personal learning and reference purposes only.
You may not copy, modify, redistribute, sell, or claim this project as your own without prior written permission from the author.

See the **`LICENSE`** file for full details.

**Author:** Ivan Susendra
