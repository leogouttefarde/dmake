pdf("sujet_makefiles_blender_2.59_Makefile_nth1.pdf")

nCPU = 40

STEP = 8

#x_axis <- seq.int( nCPU, 1, -1 )
x_axis <- seq.int( STEP, nCPU, STEP )

data <- c( 61631,  53256,  50893,  48219,  40123 )
data = data / 1000

# -1 )
#data <- head( data, -1)

#sCPU = tail(data, n=1)

xLim = 2 * nCPU / 3
sCPU = head(data, n=1)
yLim = sCPU + sCPU / 100

x_ideal <- c( STEP, nCPU )
y_ideal <- c( sCPU, STEP * sCPU / nCPU )
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
