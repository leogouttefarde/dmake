


# reset network files
for SERV in $(cat ~/nodes); do

  # reset blender_2.49
  ssh -oStrictHostKeyChecking=no root@$SERV 'cd ~/make/sujet/makefiles/blender_2.49 && rm frame_*.png cube_anim.blend cube.mpg'

done


