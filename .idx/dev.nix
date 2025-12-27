# To learn more about how to use Nix to configure your environment
# see: https://firebase.google.com/docs/studio/customize-workspace
{ pkgs, ... }: {
  # Which nixpkgs channel to use.
  channel = "stable-24.05"; # or "unstable"

  # Use https://search.nixos.org/packages to find packages
  packages = [  
    
    
    pkgs.jdk17             # Tetap gunakan JDK 17
    pkgs.android-tools     # Menyediakan adb, sdkmanager, dll
    pkgs.gradle            # Versi gradle sistem
    pkgs.unzip
  ];

  # Sets environment variables in the workspace
  env = {
    JAVA_HOME = "${pkgs.jdk17}";
    ANDROID_SDK_ROOT = "/home/user/Android/Sdk";
    ANDROID_HOME = "/home/user/Android/Sdk";
    # Cara benar menerima lisensi di terminal IDX lewat environment variable
    NIXPKGS_ACCEPT_ANDROID_SDK_LICENSE = "1";
  };

  idx = {
    # Search for the extensions you want on https://open-vsx.org/ and use "publisher.id"
    extensions = [
      "muhammad-sammy.android-launcher-idx"
    ];

    # Enable previews (dimatikan agar ringan di HP)
    previews = {
      enable = false;
    };

    # Workspace lifecycle hooks
    workspace = {
      # Runs when a workspace is first created
      onCreate = {
        # Membuat folder SDK dan file local.properties otomatis
        setup-sdk = "mkdir -p /home/user/Android/Sdk && echo 'sdk.dir=/home/user/Android/Sdk' > local.properties";
      };
      # Runs when the workspace is (re)started
      onStart = {
        # Memastikan adb server jalan jika dibutuhkan
        # start-adb = "adb start-server";
      };
    };
  };
}