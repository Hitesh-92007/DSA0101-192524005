"""Create class diagrams and clearly labelled output images from saved run logs."""
from pathlib import Path
from PIL import Image, ImageDraw, ImageFont

ROOT = Path(__file__).resolve().parents[1]
OUT = ROOT / "Supporting_Files"
FONT_PATHS = [r"C:\Windows\Fonts\arial.ttf", r"C:\Windows\Fonts\segoeui.ttf"]
FONT_BOLD_PATHS = [r"C:\Windows\Fonts\arialbd.ttf", r"C:\Windows\Fonts\segoeuib.ttf"]


def font(size, bold=False):
    for path in (FONT_BOLD_PATHS if bold else FONT_PATHS):
        try:
            return ImageFont.truetype(path, size)
        except OSError:
            pass
    return ImageFont.load_default()


def box(draw, rect, title, rows, width=4):
    x1, y1, x2, y2 = rect
    draw.rounded_rectangle(rect, radius=16, fill="#f6f9fc", outline="#18374f", width=width)
    title_h = 54
    draw.rounded_rectangle((x1, y1, x2, y1 + title_h), radius=14, fill="#dceaf4", outline="#18374f", width=width)
    draw.rectangle((x1, y1 + title_h - 15, x2, y1 + title_h), fill="#dceaf4")
    draw.text((x1 + 16, y1 + 10), title, font=font(28, True), fill="#102a3a")
    y = y1 + title_h + 10
    for row in rows:
        draw.text((x1 + 18, y), row, font=font(22), fill="#203746")
        y += 31
    return (x1, y1, x2, y2)


def arrow(draw, start, end, label=None, dashed=False):
    x1, y1 = start; x2, y2 = end
    if dashed:
        dx, dy = x2 - x1, y2 - y1
        length = max(1, (dx * dx + dy * dy) ** .5)
        ux, uy = dx / length, dy / length
        at = 0
        while at < length - 22:
            stop = min(at + 14, length - 22)
            draw.line((x1 + ux * at, y1 + uy * at, x1 + ux * stop, y1 + uy * stop), fill="#bc5d38", width=4)
            at += 24
    else:
        draw.line((x1, y1, x2, y2 - 18), fill="#18374f", width=4)
    # Hollow triangle points to the base class.
    tri = [(x2, y2), (x2 - 16, y2 - 24), (x2 + 16, y2 - 24)]
    draw.polygon(tri, fill="white", outline="#18374f")
    if label:
        draw.text(((x1 + x2) // 2 + 10, (y1 + y2) // 2 - 8), label, font=font(18, True), fill="#365b72")


def make_task1():
    im = Image.new("RGB", (1500, 720), "white"); d = ImageDraw.Draw(im)
    d.text((50, 25), "Task 1  Payroll class hierarchy", font=font(34, True), fill="#102a3a")
    box(d, (500, 110, 1000, 315), "Staff (abstract)", ["- name : string", "- id : int", "+ pay() : double = 0"])
    box(d, (75, 500, 465, 680), "Professor", ["- basic, research", "+ pay() override"])
    box(d, (555, 500, 945, 680), "LabAssistant", ["- basic", "+ pay() override"])
    box(d, (1035, 500, 1425, 680), "GuestFaculty", ["- hours, rate", "+ pay() override"])
    arrow(d, (270, 500), (650, 315)); arrow(d, (750, 500), (750, 315)); arrow(d, (1230, 500), (850, 315))
    d.text((50, 690), "All three derived types share the Staff interface; each defines its own pay rule.", font=font(19), fill="#526979")
    im.save(ROOT / "Task01" / "UML.png")


def make_task2():
    im = Image.new("RGB", (1600, 930), "white"); d = ImageDraw.Draw(im)
    d.text((50, 25), "Task 2  People hierarchy and virtual diamond", font=font(34, True), fill="#102a3a")
    box(d, (570, 90, 1030, 270), "Person", ["- name, id, phone"])
    box(d, (185, 390, 610, 570), "Student", ["- programme, CGPA", "/ virtual public Person"])
    box(d, (990, 390, 1415, 570), "Staff", ["- department, salary", "/ virtual public Person"])
    box(d, (550, 700, 1050, 880), "TeachingAssistant", ["Student + Staff", "/ one shared Person"])
    box(d, (40, 700, 430, 880), "ResearchScholar", ["Student + fellowship"])
    arrow(d, (395, 390), (700, 270), "virtual")
    arrow(d, (1205, 390), (900, 270), "virtual")
    arrow(d, (690, 700), (455, 570))
    arrow(d, (910, 700), (1200, 570))
    arrow(d, (235, 700), (360, 570))
    d.text((50, 895), "Student and Staff share one Person base in TeachingAssistant; ResearchScholar extends Student.", font=font(19), fill="#526979")
    im.save(ROOT / "Task02" / "UML.png")


def make_task3():
    im = Image.new("RGB", (1500, 720), "white"); d = ImageDraw.Draw(im)
    d.text((50, 25), "Task 3  Member classes and construction order", font=font(34, True), fill="#102a3a")
    box(d, (450, 100, 990, 270), "Person", ["- name, age", "- home : Address"])
    box(d, (60, 450, 390, 620), "Address", ["- city, pin"])
    box(d, (435, 450, 765, 620), "Student", ["- registration number", "- course : Course"])
    box(d, (810, 450, 1140, 620), "Staff", ["- department"])
    box(d, (1180, 450, 1470, 620), "Course", ["- title"])
    arrow(d, (590, 450), (630, 270), "virtual base")
    arrow(d, (975, 450), (840, 270), "virtual base")
    # Composition diamonds and member links.
    d.line((450, 180, 390, 180, 390, 525), fill="#18374f", width=4)
    d.polygon([(390, 525), (380, 510), (390, 495), (400, 510)], fill="#18374f", outline="#18374f")
    d.line((765, 535, 1180, 535), fill="#18374f", width=4)
    d.polygon([(765, 535), (780, 525), (795, 535), (780, 545)], fill="#18374f", outline="#18374f")
    d.text((50, 660), "Address is a Person member; Course is a Student member. Bases build first; members follow declaration order.", font=font(18), fill="#526979")
    im.save(ROOT / "Task03" / "UML.png")


def make_task6():
    im = Image.new("RGB", (1850, 1180), "white"); d = ImageDraw.Draw(im)
    d.text((50, 25), "Task 6  Vehicle rental hierarchy", font=font(34, True), fill="#102a3a")
    box(d, (680, 80, 1170, 285), "Vehicle (abstract)", ["registration, daily rate", "+ rent / return", "+ rentalCharge() = 0"])
    box(d, (70, 425, 430, 595), "Bicycle", ["daily charge"])
    box(d, (600, 395, 1050, 615), "MotorVehicle", ["energy level", "service due date"])
    box(d, (1370, 425, 1780, 595), "ElectricBus", ["new extension type", "kWh charge"])
    box(d, (270, 790, 650, 960), "TwoWheeler", ["fuel charge"])
    box(d, (755, 735, 1165, 905), "Car (abstract)", ["seating capacity"])
    box(d, (650, 1000, 1030, 1145), "FuelCar", ["litre charge"])
    box(d, (1120, 1000, 1510, 1145), "ElectricCar", ["battery kWh"])
    arrow(d, (250, 425), (770, 285))
    arrow(d, (820, 395), (900, 285))
    arrow(d, (1560, 425), (1040, 285), dashed=True, label="added later")
    arrow(d, (460, 790), (690, 615))
    arrow(d, (960, 735), (850, 615))
    arrow(d, (840, 1000), (840, 905))
    arrow(d, (1320, 1000), (1080, 905))
    d.text((50, 1160), "ServiceDate is a member value of MotorVehicle. New vehicle classes implement Vehicle's virtual interface.", font=font(19), fill="#526979")
    im.save(ROOT / "Task06" / "UML.png")


def make_output_capture(task):
    source = ROOT / task / "RUN_OUTPUT.txt"
    if not source.exists():
        return
    text = source.read_text(encoding="utf-8-sig", errors="replace").splitlines()
    title_font = font(25, True); mono = font(18)
    wrapped = []
    for line in text:
        if len(line) <= 110:
            wrapped.append(line)
        else:
            for i in range(0, len(line), 110):
                wrapped.append(line[i:i + 110])
    line_h = 25; width = 1350; top = 85; height = min(5000, top + line_h * max(1, len(wrapped)) + 35)
    im = Image.new("RGB", (width, height), "#101820"); d = ImageDraw.Draw(im)
    d.rectangle((0, 0, width, 60), fill="#1d3548")
    d.text((28, 15), f"{task} program output - generated from saved run log", font=title_font, fill="#f1f5f9")
    for i, line in enumerate(wrapped[:(height - top - 20) // line_h]):
        d.text((30, top + i * line_h), line, font=mono, fill="#d8eadc")
    im.save(ROOT / task / "output_capture.png")


def main():
    make_task1(); make_task2(); make_task3(); make_task6()
    for task in [f"Task{i:02}" for i in range(1, 7)]:
        make_output_capture(task)
    print("Created 4 diagrams and 6 output captures from the saved program logs.")


if __name__ == "__main__":
    main()
