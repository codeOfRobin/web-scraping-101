defmodule Scraper do
  
  def scrape(src) do
    body = HTTPoison.get!(src).body
    body 
      |> Floki.find(".storylink")
      |> Floki.attribute("href")
      |> Enum.map(fn(link) -> IO.puts "#{link}\n" end)

  end

  def main(args) do
    scrape("https://news.ycombinator.com")
  end

end
