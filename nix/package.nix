{ pkgs }:

pkgs.stdenv.mkDerivation {
  pname = "libce";
  version = "1.0.0";

  src = ../.;

  nativeBuildInputs = [
    pkgs.gnumake
  ];

  strictDeps = true;
  doCheck = true;

  buildPhase = ''
    make
  '';

  checkPhase = ''
    make check
  '';

  installPhase = ''
    make install PREFIX=$out
  '';

  meta = {
    description = "Extended C environment built around explicit ownership and predictable failure";
    homepage = "https://github.com/Censera/Ce";
    license = pkgs.lib.licenses.asl20;
    platforms = pkgs.lib.platforms.unix;
  };
}
