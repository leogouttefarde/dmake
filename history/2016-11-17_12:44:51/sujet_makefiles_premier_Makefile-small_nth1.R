pdf("sujet_makefiles_premier_Makefile-small_nth1.pdf")

nCPU = 40

STEP = 8

#x_axis <- seq.int( nCPU, 1, -1 )
x_axis <- seq.int( STEP, nCPU, STEP )

data <- c( 25922,  15118,  4275,  3826,  4231 )
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

