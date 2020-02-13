let
  pkgs = import <nixpkgs> {
    crossSystem = {
      config = "armv7l-unknown-linux-gnueabihf";
      platform = pkgs.lib.systems.platforms.armv7l-hf-multiplatform;
    };
  };
  inherit (pkgs) lib buildUBoot;

in rec {
  ubootNgenicAlfred = buildUBoot rec {
    name = "u-boot-ngenic-alfred";
    src = ../.;

    defconfig = "ngenic_alfred_defconfig";

    filesToInstall = [ "u-boot.imx" ];

    postInstall = ''
      mv $out/u-boot.imx $out/u-boot-ngenic-alfred.imx
    '';

    extraMeta = with lib; {
      maintainers = [ maintainers.mattias01 ];
    };
  };

  ubootNgenicAlfredBootbomb = buildUBoot rec {
    name = "u-boot-ngenic-alfred-bootbomb";
    src = ../.;

    defconfig = "ngenic_alfred_bootbomb_defconfig";

    filesToInstall = [ "u-boot.imx" ];

    postInstall = ''
      mv $out/u-boot.imx $out//u-boot-ngenic-alfred-bootbomb.imx
    '';

    extraMeta = with lib; {
      maintainers = [ maintainers.mattias01 ];
    };
  };

  ubootNgenicBohr = buildUBoot rec {
    name = "u-boot-ngenic-bohr";
    src = ../.;

    defconfig = "ngenic_bohr_defconfig";

    filesToInstall = [ "u-boot.imx" ];

    postInstall = ''
      mv $out/u-boot.imx $out/u-boot-ngenic-bohr.imx
    '';

    extraMeta = with lib; {
      maintainers = [ maintainers.mattias01 ];
    };
  };

  ubootNgenicBohrBootbomb = buildUBoot rec {
    name = "u-boot-ngenic-bohr-bootbomb";
    src = ../.;

    defconfig = "ngenic_bohr_bootbomb_defconfig";

    filesToInstall = [ "u-boot.imx" ];

    postInstall = ''
      mv $out/u-boot.imx $out//u-boot-ngenic-bohr-bootbomb.imx
    '';

    extraMeta = with lib; {
      maintainers = [ maintainers.mattias01 ];
    };
  };

}
