pdf("sujet_makefiles_premier_Makefile_nth8.pdf")

nCPU = 42
STEP = 3

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

data <- c( 348262,  106439,  106424,  106446,  106448,  106465,  106437,  106445,  106447,  106438,  106437,  106466,  106438,  106447,  106420 )

# -1 )
#data <- head( data, -1)

data = data / 1000

#sCPU = tail(data, n=1)

xLim = 4 * nCPU / 7
sCPU = head(data, n=1)
yLim = sCPU + sCPU / 100

x_ideal <- c( 1, nCPU )
y_ideal <- data / x_axis
y_lims <- c( min(data), yLim )

ideal <- function( n ) {
  return (sCPU/n)
}

plot( ideal, 1, nCPU, ann=FALSE, col="cyan" )
lines( x_axis, data, pch="+", cex=1.2, type="o", col="dark blue", ann=FALSE, ylim=y_lims )

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

m = nCPU

T1 = sCPU

Tm = data

acc = T1 / Tm


travail=c()
for (i in 1:length(data)) {
  travail = c( travail, x_axis[i] * Tm[i] )
}

eff=T1/travail



prefix="graphe"

pdf(paste(prefix,"_acc.pdf", sep=""))

plot( x_axis, acc, pch="+", cex=1.2, type="o", col="dark blue", ann=FALSE ) #, ylim=y_lims )

title( main="Accélération", font.main=1, cex.main=2 )
title( xlab="Nombre de machines", cex.lab=1.2 )
title( ylab="Accélération", cex.lab=1.2 )

dev.off()



pdf(paste(prefix,"_eff.pdf", sep=""))

plot( x_axis, eff, pch="+", cex=1.2, type="o", col="dark blue", ann=FALSE ) #, ylim=y_lims )

title( main="Efficacité", font.main=1, cex.main=2 )
title( xlab="Nombre de machines", cex.lab=1.2 )
title( ylab="Efficacité", cex.lab=1.2 )

dev.off()


pdf(paste(prefix,"_work.pdf", sep=""))

plot( x_axis, travail, pch="+", cex=1.2, type="o", col="dark blue", ann=FALSE ) #, ylim=y_lims )

title( main="Travail", font.main=1, cex.main=2 )
title( xlab="Nombre de machines", cex.lab=1.2 )
title( ylab="Travail", cex.lab=1.2 )

dev.off()
