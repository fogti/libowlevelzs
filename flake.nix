{
  description = "Zscheile Lowlevel (utility) library";
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/master";
    yz-flake-utils.url = "github:YZITE/flake-utils";
  };
  outputs = { nixpkgs, yz-flake-utils, ... }:
    yz-flake-utils.lib.mkFlakeFromProg {
      prevpkgs = nixpkgs;
      progname = "crulz";
      drvBuilder = final: prev: prev.callPackage ./. { };
    };
}
