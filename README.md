# Cloud detection with custom k-means

Implementation in C of the paper
[Introducing spatial information in k-means algorithm for clouds detection in optical satellite images](http://adsabs.harvard.edu/abs/2001SPIE.4168...67B).

A copy of the pdf is included at the root of the repo.

## The algorithm

In a few words, we need a way to compute the % of clouds in an photo taken from space.

The algorithm creates blob of pixels (central, up, right, down, left) and applies
a k-medians on it.

The k-medians is like a k-means but uses a median operation instead of a mean
operation. Furthermore the distance metric used is a Manhattan distance.

The pixels belongings to the highest k-medians' center (thus having pixels with
high values -> whiter) are considered as clouds.

## Results

![20342579204091110141x.bmp](IMAGES/20342579204091110141x.bmp)

```
Image: 20342579204091110141x.bmp	Percentage of clouds:	0.00 %
```

![20352579703071128161x.bmp](IMAGES/20352579703071128161x.bmp)

```
Image: 20352579703071128161x.bmp	Percentage of clouds:	8.97 %
```

![20422629111221044022x.bmp](IMAGES/20422629111221044022x.bmp)

```
Image: 20422629111221044022x.bmp	Percentage of clouds:	39.94 %
```

![20422629202281059191p.bmp](IMAGES/20422629202281059191p.bmp)

```
Image: 20422629202281059191p.bmp	Percentage of clouds:	13.49 %
```

![20422629407061038501x.bmp](IMAGES/20422629407061038501x.bmp)

```
Image: 20422629407061038501x.bmp	Percentage of clouds:	47.49 %
```

![30312549607241130511x.bmp](IMAGES/30312549607241130511x.bmp)

```
Image: 30312549607241130511x.bmp	Percentage of clouds:	24.65 %
```

![36022919509261649571x.bmp](IMAGES/36022919509261649571x.bmp)

```
Image: 36022919509261649571x.bmp	Percentage of clouds:	20.17 %
```

![36293009311171548281x.bmp](IMAGES/36293009311171548281x.bmp)

```
Image: 36293009311171548281x.bmp	Percentage of clouds:	19.39 %
```

![36683199502211443252x.bmp](IMAGES/36683199502211443252x.bmp)

```
Image: 36683199502211443252x.bmp	Percentage of clouds:	0.00 %
```

## Disclamer

This project was done during a course about *Teledetection* at *EPITA*. The
teacher asked explicitely to use the files `main.c` & `makefile`, and the
weird architecture. I would have done a bit differently.
