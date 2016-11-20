

for file in history/*/*.pdf ; do

  echo "Converting ${file} to PNG ..."
  convert -flatten -density 700 "${file}" -resize 1000 "${file}".png
  # convert -density 700 "${file}" -resize 1000 "${file}".png

done

