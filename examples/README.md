# How to run this examples

```bash
mruby simple.rb > simple.ppm
```

```bash
mruby mandelbrot.rb > m.ppm
```

```bash
mruby mandelbrot_anim.rb |ffmpeg -f ppm_pipe -r 12 -s 256x256 -i - -r 12 -pix_fmt yuv420p ma.mp4
```
