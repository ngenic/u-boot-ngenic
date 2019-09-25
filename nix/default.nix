let
  pkgs = import <nixpkgs> {
    crossSystem = {
      config = "armv7l-unknown-linux-gnueabihf";
      platform = pkgs.lib.systems.platforms.armv7l-hf-multiplatform;
    };
  };
  inherit (pkgs) lib buildUBoot;

in rec {
  ubootBootbombMYDY6ULL = buildUBoot rec {
    name = "u-boot-bootbomb-myd-y6ull";
    src = ../.;

    defconfig = "myd_y6ull_14x14_emmc_bootbomb_defconfig";

#   # Start the usb mass storage gadget on boot.
#   extraConfig = ''
#     CONFIG_USE_BOOTCOMMAND=y
#     CONFIG_BOOTCOMMAND="ums 0 mmc 1"
#   '';

    filesToInstall = [ "u-boot.imx" ];

    postInstall = ''
      mv $out/u-boot.imx $out/u-boot-bootbomb-myd-y6ull.imx
    '';

    extraMeta = with lib; {
      maintainers = [ maintainers.olcai ];
    };
  };

  ubootPicoIMX6UL = buildUBoot rec {
    name = "u-boot-pico-imx6ul";
    src = ../.;

    defconfig = "pico-imx6ul_defconfig";

    filesToInstall = [ "u-boot.imx" ];

    postInstall = ''
      mv $out/u-boot.imx $out/u-boot-pico-imx6ul.imx
    '';

    extraMeta = with lib; {
      maintainers = [ maintainers.olcai ];
    };
  };
}
