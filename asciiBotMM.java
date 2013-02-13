import java.awt.Color;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.io.PrintWriter;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.swing.*;

public class asciiBotMM {

    static menuCell[] mCellArray = new menuCell[5];
    static int mNum = 0;
    static cell[][] cellArray;

    public static void main(String[] args) {
        Thread Th1 = new Thread() {

            @Override
            public void run() {
                GetValGui valG = new GetValGui();
                valG.Build();
            }
        };

        Th1.start();
    }

    static public class GetValGui {

        JTextField xField;
        JTextField yField;

        public void Build() {
            JFrame f = new JFrame();
            JPanel p = new JPanel(null);

            JButton InpButton = new JButton();
            ExitButtonListener bilistener = new ExitButtonListener(f);
            InpButton.addMouseListener(bilistener);

            JLabel inflabl = new JLabel();
            xField = new JTextField(10);
            yField = new JTextField(10);

            xField.setBounds(30, 40, 150, 30);
            yField.setBounds(30, 80, 150, 30);

            JLabel xLabel = new JLabel();
            JLabel yLabel = new JLabel();

            xLabel.setBounds(190, 40, 120, 30);
            yLabel.setBounds(190, 80, 120, 30);

            xLabel.setText("X axis size");
            yLabel.setText("Y axis size");

            xLabel.setOpaque(true);
            yLabel.setOpaque(true);

            inflabl.setBounds(30, 10, 230, 30);
            inflabl.setText("Around (20,20) works quite well");

            InpButton.setBounds(30, 130, 200, 30);
            InpButton.setText("Enter Choices");


            p.add(xField);
            p.add(yField);
            p.add(xLabel);
            p.add(yLabel);
            p.add(InpButton);
            p.add(inflabl);

            f.getContentPane().add(p);
            f.setTitle("Enter inital map info");
            f.setSize((290), (200));
            f.setResizable(true);
            f.setLocationRelativeTo(null);
            f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
            f.setVisible(true);
        }

        class ExitButtonListener extends MouseAdapter {

            private final JFrame frame;

            ExitButtonListener(JFrame frame) {
                this.frame = frame;
            }

            @Override
            public void mousePressed(MouseEvent me) {
                int xSize = Integer.parseInt(xField.getText());
                int ySize = Integer.parseInt(yField.getText());

                bmGUI(xSize, ySize);
                frame.dispose();
            }
        }
    }

    public static void bmGUI(int xM, int yM) {
        int i, j;
        JFrame f = new JFrame();
        JPanel p = new JPanel(null);
        cellArray = new cell[xM][yM];

        for (i = 0; i < 5; ++i) {
            mCellArray[i] = new menuCell(i, xM);
            p.add(mCellArray[i]);
        }
        for (i = 0; i < xM; ++i) {

            for (j = 0; j < yM; ++j) {
                cellArray[i][j] = new cell(0, i, j);
                p.add(cellArray[i][j]);
            }
        }

        sListener bslistener = new sListener();

        JButton savebutton = new JButton();
        savebutton.addMouseListener(bslistener);
        savebutton.setBounds(0, (yM + 1) * 30, xM * 30, 50);
        savebutton.setText("Save Map");

        p.add(savebutton);

        f.getContentPane().add(p);
        f.setTitle("Ascii bots GUI");
        f.setSize((30 * xM), 30 * (yM + 1) + 75);
        f.setResizable(true);
        f.setLocationRelativeTo(null);
        f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        f.setVisible(true);
    }

    public static class sListener extends MouseAdapter {

        @Override
        public void mousePressed(final MouseEvent me) {
            final JFileChooser fc = new JFileChooser();
            if (fc.showSaveDialog(me.getComponent()) == JFileChooser.APPROVE_OPTION) {
                File file = fc.getSelectedFile();
                try {
                    PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(file)));
                    out.println(Integer.toString(cellArray.length));
                    out.println(Integer.toString(cellArray[1].length));

                    for (int i = 0; i < cellArray.length; ++i) {
                        for (int j = 0; j < cellArray[i].length; ++j) {
                            if (cellArray[i][j].type != 0) {
                                out.println(Integer.toString(cellArray[i][j].type) + " " + Integer.toString(cellArray[i][j].x) + " " + Integer.toString(cellArray[i][j].y) + " " + Integer.toString(cellArray[i][j].direction));
                            }
                        }
                    }
                    out.flush();
                } catch (IOException ex) {
                    Logger.getLogger(asciiBotMM.class.getName()).log(Level.SEVERE, null, ex);
                }
            }


        }
    }

    public static class menuCellListener extends MouseAdapter {

        @Override
        public void mousePressed(final MouseEvent me) {
            if (((menuCell) (me.getSource())).isCurrent) {
                ((menuCell) (me.getSource())).isCurrent = false;
                ((menuCell) (me.getSource())).setBackground(Color.RED);
                mNum = 0;
            } else {
                for (int i = 0; i < 5; ++i) {
                    mCellArray[i].isCurrent = false;
                    mCellArray[i].setBackground(Color.RED);
                }
                mNum = ((menuCell) (me.getSource())).num;
                ((menuCell) (me.getSource())).isCurrent = true;
                ((menuCell) (me.getSource())).setBackground(Color.ORANGE);
            }
        }
    }

    static class menuCell extends JLabel {

        int num;
        boolean isCurrent = false;

        menuCell(int i, int xNum) {
            menuCellListener mcelllistener = new menuCellListener();
            this.setOpaque(true);
            this.setBounds(30 * i * (xNum / 5), 0, 30 * (xNum / 5), 30);
            this.setBackground(Color.RED);
            this.setHorizontalTextPosition(RIGHT);
            this.setBorder(BorderFactory.createLineBorder(Color.black));
            this.addMouseListener(mcelllistener);
						if(i == 2){
							i = 5;
						}
						num = i;

            switch (i) {
                case 0:
                    this.setText("Empty " + Integer.toString(i));
                    break;
                case 1:
                    this.setText("Wall " + Integer.toString(i));
                    break;
                case 5:
                    this.setText("Player " + Integer.toString(i));
                    break;
                case 3:
                    this.setText("Bullet " + Integer.toString(i));
                    break;
                case 4:
                    this.setText("Mine " + Integer.toString(i));
                    break;

            }
        }
    }

    public static class cellListener extends MouseAdapter {

        @Override
        public void mousePressed(final MouseEvent me) {


            if (mNum == 0) {
                ((cell) (me.getSource())).type = mNum;
                ((cell) (me.getSource())).setText("");
            } else {
                if (((cell) (me.getSource())).type == mNum && ((cell) (me.getSource())).direction != 8) {
                    ((cell) (me.getSource())).direction = ((cell) (me.getSource())).direction + 2;
                    ((cell) (me.getSource())).setText(Integer.toString(mNum) + " " + Integer.toString(((cell) (me.getSource())).direction));
                } else {
                    ((cell) (me.getSource())).type = mNum;
                    ((cell) (me.getSource())).direction = 2;
                    ((cell) (me.getSource())).setText(Integer.toString(mNum) + " " + Integer.toString(((cell) (me.getSource())).direction));
                }
            }
        }
    }

    static class cell extends JLabel {

        int type;
        int x;
        int y;
        int direction;

        cell(int t, int i, int j) {
            type = t;
            x = i;
            y = j;
            direction = 2;
            cellListener celllistener = new cellListener();
            this.setOpaque(true);
            this.setBounds(30 * i, (30 * j) + 30, 30, 30);
            this.setBackground(Color.BLACK);
            this.setForeground(Color.red);
            this.setBorder(BorderFactory.createLineBorder(Color.WHITE));
            this.addMouseListener(celllistener);
        }
    }
}
