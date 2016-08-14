# How to run this examples

```bash
mruby simple.rb > simple.ppm
```

```bash
mruby mandelbrot.rb > m.ppm
```

## mandelbrot_anim.rb

```bash
mruby mandelbrot_anim.rb |ffmpeg -f ppm_pipe -r 12 -s 256x256 -i - -r 12 -pix_fmt yuv420p ma.mp4
```

or

```bash
mruby mandelbrot_anim.rb |ffmpeg -f image2pipe -vcodec ppm -r 12 -s 256x256 -i - -r 12 -pix_fmt yuv420p ma.mp4
```
