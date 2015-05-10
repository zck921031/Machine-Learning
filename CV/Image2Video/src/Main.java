import java.io.*;
import java.util.*;
import java.util.Map.Entry;

public class Main {
	public String ffmpegPath;
	public String imagePath;
	boolean debug = false;
	String subfix = ".jpg";
	TreeMap<String, TreeSet<String> > que = new TreeMap<String, TreeSet<String> >();
	public void listJavaFile(String Root, String prefix) {
		File file = new File(Root);
		if ( file.isDirectory() ) {
			File[] files = file.listFiles();
			TreeSet<String> imgs = new TreeSet<String>();
			
			for (int i = 0; i < files.length; i++) {
				String name = files[i].getName();
				if ( name.trim().toLowerCase().endsWith(subfix) ) {
					//System.out.println( name );
					imgs.add( name );
				}
				if ( files[i].isDirectory() ) {
					String path = files[i].getPath();
					listJavaFile(path, prefix+"."+name );
				}
			}
			
			if ( !imgs.isEmpty() ){
				//System.out.println( Root );
				//System.out.println( prefix );
				que.put(Root, imgs);
			}
		}else{
			System.out.println( Root + " is not an existed folder." );			
		}
	}
	
	void image2video(String Root, TreeSet<String>imgs, String videoName){
		del("temp");
		System.out.println(Root);
		File file = new File("temp");
		file.mkdirs();
		int N = imgs.size();
		int bit = 0, tmp = N;
		while( tmp>0 ){
			tmp /= 10;
			bit++;
		}
		if (bit==0) return ;
		
		int i = 1;
		Runtime run = Runtime.getRuntime();	
		for( String img : imgs ){
			String fileName = Root + "\\" + img;
			String linkName = Integer.toString(i);
			while ( linkName.length() < bit ){
				linkName = "0" + linkName;
			}
			linkName += subfix;
			linkName = "temp\\"+linkName;
			if (debug){
				System.out.println( img );
				System.out.println( linkName );
			}
			i++;
			
			//run mklink
			try {
				Process process = run.exec("cmd /c mklink " + linkName + "  " + fileName );				
				process.waitFor();
			} catch (Exception e) {
				e.printStackTrace();
			}
		}		
		System.out.println("mklink finished~");
		
		String command = ffmpegPath + " -loglevel quiet -i temp/%" + Integer.toString(bit) 
				+ "d.jpg -vcodec libx264 -s 2048x2048 " + videoName;
		//String command = "ffmpeg.exe  -i temp/%" + Integer.toString(bit) + "d.jpg -vcodec libx264 -s 2048x2048 " + videoName;
		System.out.println( command );
		
		//run ffmpeg encoder
		try {
			Process process = run.exec( command );
//			String s;
//			BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(process.getInputStream()) );
//			while((s=bufferedReader.readLine()) != null)
//				System.out.println(s);
			process.waitFor();
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		del("temp");
	}
	
	public void solve(){
		for(Entry<String, TreeSet<String>> entry:que.entrySet() ){
			
			//System.out.println(entry.getKey()+" ---> "+entry.getValue());
			
			String Root = entry.getKey();
			String prefix;
			prefix = Root.replaceAll("[A-Z]:\\\\", "");
			prefix = prefix.replaceAll("\\\\", ".");

			//System.out.println(prefix+" ---> "+entry.getValue());	        
			image2video(Root, entry.getValue(), prefix+".mpg");			
		}
		
	}
	public static void main(String[] args) {

		System.out.println( args.length );
		if ( args.length < 2 ){
			System.out.println("usage: java -jar Image2Video.jar ffmpegPath imagePath");
			return ;
		}
		System.out.println("ffmpegPath: " + args[0]);
		System.out.println("imagePath: " + args[1]);
		Main worker = new Main();
		worker.ffmpegPath = args[0];
		worker.imagePath = args[1];
		//worker.listJavaFile("C:\\Users\\zck\\Desktop\\新建文件夹\\毕业设计\\数据", "flare");
		worker.listJavaFile(worker.imagePath, "flare");		
		worker.solve();
	}

	public void del(String filepath){
		File f = new File(filepath);
		if( f.exists() ){
			File delFile[]=f.listFiles();
			int n=delFile.length;
			for(int j=0;j<n;j++){
				if(delFile[j].isDirectory()){
					del( delFile[j].getAbsolutePath() );
				}else{
					delFile[j].delete();//删除文件
				}
			}
			f.delete();
		}
	}
	
}
