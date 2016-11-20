pdf("sujet_makefiles_blender_2.59_Makefile_nth1.pdf")

nCPU = 28
STEP = 7

#x_axis <- seq.int( nCPU, STEP, -STEP )
#x_axis <- seq.int( STEP, nCPU, STEP )
x_axis=c()
for (i in seq(from=nCPU, to=1, by=-STEP)) {
  x_axis = c( x_axis, i )
}

if ( ! (1 %in% x_axis) ) {
  x_axis = c( x_axis, 1 )
}

x_axis = rev(x_axis)

data <- c( 122449,  24416,  19501,  11223,  36279 )

# -1 )
#data <- head( data, -1)

data = data / 1000

#sCPU = tail(data, n=1)

xLim = 4 * nCPU / 7
sCPU = head(data, n=1)
yLim = sCPU + sCPU / 100

x_ideal <- c( 1, nCPU )
y_ideal <- c( sCPU, sCPU / nCPU )
y_lims <- c( min(data), yLim )

plot( x_axis, data, pch="+", type="o", col="dark blue", ann=FALSE, ylim=y_lims )
lines( x_ideal, y_ideal, pch="+", type="o", col="cyan", ann=FALSE )

title( main="Durée de compilation", font.main=1, cex.main=2 )
title( xlab="Nombre de machines", cex.lab=1.2 )
title( ylab="Durée en secondes", cex.lab=1.2 )


# places a legend at the appropriate place
legend( xLim, yLim,

 # puts text in the legend
c("Parallélisation idéale", "Temps mesurés"),

# gives the legend appropriate symbols (lines)
lty=c(1,1),

 # gives the legend lines the correct color and width
lwd=c(2.5,2.5),col=c("cyan", "blue"),bg="white")


dev.off()
