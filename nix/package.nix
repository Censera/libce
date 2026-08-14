{ pkgs }:

pkgs.stdenv.mkDerivation {
  pname = "libce";
  version = "0.2.0";

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
    make test
  '';

  installPhase = ''
    make install PREFIX=$out
  '';

  meta = {
    description = "Dependency-free C utility library built around explicit ownership and predictable failure";
    homepage = "https://github.com/Censera/Ce";
    license = pkgs.lib.licenses.asl20;
    platforms = pkgs.lib.platforms.unix;
  };
}
