#!/usr/bin/ruby

# coding: utf-8

# ASCII で書かれたファイルをバイナリへ変換して書き出す

def conv(in_file_path, out_file_path)
  text = File.open(in_file_path).read()
  sp = text.split
  d = sp.map{|h| h.hex}
  File.open(out_file_path, "wb").write(d.pack("C*"))
end

def main(argv)
  in_file_path = argv[0]
  out_file_path = argv[1]
  
  conv(in_file_path, out_file_path)
end

main(ARGV)
