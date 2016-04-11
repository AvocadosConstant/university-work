class convertjflaptojff
{
	public static void main(String[] args)
	{
		java.io.BufferedReader input;
		java.util.TreeSet<String> acceptStates = new java.util.TreeSet<>();
		java.util.TreeSet<String> acceptIds = new java.util.TreeSet<>();
		java.util.HashMap<String, String[]> idToLocation = new java.util.HashMap<>(0);
		java.util.TreeSet<String> states = new java.util.TreeSet<>();
		java.util.TreeSet<String> ids = new java.util.TreeSet<>();
		java.util.ArrayList<String> stateList = new java.util.ArrayList<>(0);
		java.util.ArrayList<String> idList = new java.util.ArrayList<>(0);
		java.util.ArrayList<String> fromState = new java.util.ArrayList<>(0);
		java.util.ArrayList<String> fromId = new java.util.ArrayList<>(0);
		java.util.ArrayList<String> toState = new java.util.ArrayList<>(0);
		java.util.ArrayList<String> toId = new java.util.ArrayList<>(0);
		java.util.ArrayList<String> read = new java.util.ArrayList<>(0);
		java.util.ArrayList<String> pop = new java.util.ArrayList<>(0);
		java.util.ArrayList<String> push = new java.util.ArrayList<>(0);
		String startState = "";
		String startId = "";
		String inn;
		
		if( args.length != 2 )
		{
			System.out.println("format is: convertjflaptojff \"input jflap file\" \"output jff file\"");
			System.exit(0);
		}
		
		try
		{
			input = new java.io.BufferedReader(new java.io.FileReader(args[0]));
			
			while( (inn = input.readLine()) != null )
			{
				// get the accept states
				if( (inn.indexOf("<structure") >= 0) && (inn.indexOf("final_states") >= 0) )
				{
					boolean done = false;
					while( !done )
					{
						inn = input.readLine();
						if( inn.indexOf("<name>") >= 0 )
						{
							acceptStates.add(getValue(inn));
							states.add(getValue(inn));
						}
						
						if( inn.indexOf("<id>") >= 0 )
						{
							acceptIds.add(getValue(inn));
							ids.add(getValue(inn));
						}
						
						if( inn.indexOf("</structure") >= 0 )
						{
							done = true;
						}
					}
				}
				
				// get the start state
				if( (inn.indexOf("<structure") >= 0) && (inn.indexOf("start_state") >= 0) )
				{
					boolean done = false;
					while( !done )
					{
						inn = input.readLine();
						if( inn.indexOf("<name>") >= 0 )
						{
							startState = getValue(inn);
						}
						
						if( inn.indexOf("<id>") >= 0 )
						{
							startId = getValue(inn);
						}
						
						if( inn.indexOf("</structure") >= 0 )
						{
							done = true;
						}
					}
				}
				
				// get the state locations
				if( inn.indexOf("<state_point") >= 0 )
				{
					boolean done = false;
					String state = "";
					String x = "";
					String y = "";
					while( !done )
					{
						inn = input.readLine();
						if( inn.indexOf("<state>") >= 0 )
						{
							state = getValue(inn);
						}
						
						if( inn.indexOf("<x>") >= 0 )
						{
							x = getValue(inn);
						}
						
						if( inn.indexOf("<y>") >= 0 )
						{
							y = getValue(inn);
						}
						
						if( inn.indexOf("</state_point") >= 0 )
						{
							idToLocation.put(state, new String[] {x, y});
							done = true;
						}
					}
				}
				
				// get the transitions
				if( (inn.indexOf("<structure") >= 0) && (inn.indexOf("transition_set") >= 0) )
				{
					boolean done = false;
					while( !done )
					{
						inn = input.readLine();
						
						if( inn.indexOf("<pop/>") >= 0 )
						{
							pop.add("");
						}
						
						if( inn.indexOf("<pop>") >= 0 )
						{
							pop.add(getValue(inn));
						}
						
						if( inn.indexOf("<push/>") >= 0 )
						{
							push.add("");
						}
						
						if( inn.indexOf("<push>") >= 0 )
						{
							push.add(getValue(inn));
						}
						
						if( inn.indexOf("<input/>") >= 0 )
						{
							read.add("");
						}
						
						if( inn.indexOf("<input>") >= 0 )
						{
							read.add(getValue(inn));
						}
						
						if( inn.indexOf("<from>") >= 0 )
						{
							while( !done )
							{
								inn = input.readLine();
								if( inn.indexOf("<name>") >= 0 )
								{
									String state = getValue(inn);
									fromState.add(state);
									states.add(state);
									stateList.add(state);
								}
								
								if( inn.indexOf("<id>") >= 0 )
								{
									String id = getValue(inn);
									ids.add(id);
									fromId.add(id);
									idList.add(id);
								}
								
								if( inn.indexOf("</from") >= 0 )
								{
									done = true;
								}
							}
						}
						done = false;
							
						if( inn.indexOf("<to/>") >= 0 )
						{
							String state = fromState.get(fromState.size()-1);
							String id = fromId.get(fromId.size()-1);
							toState.add(state);
							toId.add(id);
						}
						
						if( inn.indexOf("<to>") >= 0 )
						{
							while( !done )
							{
								inn = input.readLine();
								if( inn.indexOf("<name>") >= 0 )
								{
									String state = getValue(inn);
									toState.add(state);
									states.add(state);
									stateList.add(state);
								}
								
								if( inn.indexOf("<id>") >= 0 )
								{
									String id = getValue(inn);
									ids.add(id);
									toId.add(id);
									idList.add(id);
								}
								
								if( inn.indexOf("</to") >= 0 )
								{
									done = true;
								}
							}
							done = false;
						}
						
						if( inn.indexOf("</structure") >= 0 )
						{
							done = true;
						}
					}
				}
			}
			
			/*
			for( int i = 0; i < fromState.size(); i++ )
			{
				System.out.println("(" + fromState.get(i) + "," + read.get(i) + "," + pop.get(i) + ")->(" + toState.get(i) + "," + push.get(i) + ")");
			}
			input.close();
			 */
			
			java.io.PrintWriter output = new java.io.PrintWriter(new java.io.BufferedWriter(new java.io.FileWriter(args[1])));
			output.println("<?xml version=\"1.0\" encoding=\"UTF-8\"?><!--Created with JFLAP 6.4.--><structure>");
			output.println("\t" + "<type>pda</type>");
			output.println("\t" + "<automaton>");
			
			for( int i = 0; i < stateList.size(); i++ )
			{
				String state = stateList.get(i);
				String id = idList.get(i);
				if( ids.contains(id) )
				{
					output.println("\t\t" + "<state id=\"" + id + "\" name=\"" + state + "\">");
					String[] location = idToLocation.get(id);
					if( location != null )
					{
						output.println("\t\t\t" + "<x>" + location[0] + "</x>");
						output.println("\t\t\t" + "<y>" + location[1] + "</y>");
					}
					else
					{
						output.println("\t\t\t" + "<x>" + (i*2) + "</x>");
						output.println("\t\t\t" + "<y>" + (i*2) + "</y>");
					}
					if( acceptIds.contains(id) )
					{
						output.println("\t\t\t" + "<final/>");
					}
					if( startId.equals(id) )
					{
						output.println("\t\t\t" + "<initial/>");
					}
					output.println("\t\t" + "</state>");
					ids.remove(id);
				}
			}
			
			for( int i = 0; i < fromState.size(); i++ )
			{
				output.println("\t\t" + "<transition>");
				output.println("\t\t\t" + "<from>" + fromId.get(i) + "</from>");
				output.println("\t\t\t" + "<to>" + toId.get(i) + "</to>");
				output.println("\t\t\t" + "<read>" + read.get(i) + "</read>");
				output.println("\t\t\t" + "<pop>" + pop.get(i) + "</pop>");
				output.println("\t\t\t" + "<push>" + push.get(i) + "</push>");
				output.println("\t\t" + "</transition>");
			}
			
			output.println("\t" + "</automaton>");
			output.println("</structure>");
			output.close();
		}
		catch(Exception e)
		{
		}
	}
	
	private static String getValue(String s)
	{
		char[] chars = s.toCharArray();
		boolean inValue = false;
		String result = "";
		for( char c : chars )
		{
			if( inValue && (c != '<') )
			{
				result = result + c;
			}
			
			if( c == '>' )
			{
				inValue = true;
			}
			
			if( inValue && (c == '<') )
			{
				return result;
			}
		}
		return result;
	}
}
