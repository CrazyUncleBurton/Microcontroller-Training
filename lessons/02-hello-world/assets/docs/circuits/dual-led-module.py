import schemdraw
import schemdraw.elements as elm

d = schemdraw.Drawing()
d += elm.Jack().label('G53', loc="left").right()
d += elm.Resistor().right().label('1K0')
d += elm.LED().down().label('LED1').fill('green')
d += elm.Ground()
d.move(dx=4, dy=3)
d += elm.Jack().label('G54', loc="left").right()
d += elm.Resistor().right().label('1K0')
d += elm.LED().down().label('LED2').fill('red')
d += elm.Ground()
d.draw()
