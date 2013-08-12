#!/usr/bin/gsh
echo "listing files before write"
ls -l
# comments are now supported
echo "executing: echo data for file | tee first | tee second | cat > third"
echo "data for file" | tee first | tee second | cat > third
echo "listing files after write"
ls -l
echo "listing files contents, executing: cat first second third"
cat first second third
echo "cleaning files"
rm first second third
echo "listing files after clean"
ls -l
echo "finished..."
