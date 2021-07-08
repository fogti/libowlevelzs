{ lib
, meson
, ninja
, stdenv
}:

stdenv.mkDerivation {
  pname = "libowlevelzs";
  version = "0.1.2";
  src = ./.;
  nativeBuildInputs = [ meson ninja ];

  meta = with lib; {
    description = "Zscheile Lowlevel (utility) library";
    homepage = "https://github.com/zseri/libowlevelzs";
    license = licenses.mit;
    maintainers = with maintainers; [ zseri ];
    platforms = platforms.all;
  };
}
