# 42_little_penguin

### Troubleshoting
 
 For this error : make[1]: *** /lib/modules/4.16.9/build: No such file or directory.  Stop.

you need to make with modules_install in argument, in kernel folder

  $ make modules_install
